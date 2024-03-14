#include "AppProductExtension.h"
#include "AppMetaFlash.h"
#include "AppSourceExtension.h"
#include "AppTest.h"
#include "BasePLCDevice.h"
#include "BaseProduct.h"
#include "CameraManager.h"
#include "Logger.h"
#include "Permission.h"
#include "PrinterDevice.h"
#include "WebManager.h"
#include <cstdint>
#include <json/value.h>
#include <qimage.h>
#include <qtypes.h>
#include <string>

static float removeNum = 1;
void AppFrame::AppProductionExtension::afterProductIn(const uint32_t count)
{
    Utils::asyncTask([count] {
        const auto &product = appFramework().getPlugin<BaseProduct>();
        product->createProduct(count);
    });
}
void AppFrame::AppProductionExtension::afterProductOut(const uint32_t count)
{
    Utils::asyncTask([count] {
        const auto &product = appFramework().getPlugin<BaseProduct>();
        auto delProducts = product->deleteProduct(count);

        const QString locationPrefix = QStringLiteral("location_");
        const QString checkPrefix = QStringLiteral("check_");
        const QString ocrPrefix = QStringLiteral("ocr_");
        const QString suffix = QStringLiteral(".jpg");
        const QString separator = QStringLiteral("_");

        for (auto ptr : delProducts)
        {
            const QString idNumber = QString::number(ptr->bottleNumber());
            const auto PathNormal = AppSourceExtension::getFilePath(ResourcePath::PathImgNormal, idNumber);
            const QImage &location =
                Utils::matToQImage(ptr->getValue<ProductImgaeData>(ProductItemKey::location_path).image_);
            const auto &loc_result =
                locationPrefix + ptr->getValue<std::string>(ProductItemKey::location_result).c_str() + suffix;

            const QImage &check =
                Utils::matToQImage(ptr->getValue<ProductImgaeData>(ProductItemKey::check_path).image_);
            const auto &check_result =
                checkPrefix + ptr->getValue<std::string>(ProductItemKey::check_result).c_str() + suffix;

            const QImage &ocr = Utils::matToQImage(ptr->getValue<ProductImgaeData>(ProductItemKey::ocr_path).image_);
            const auto &ocr_result = ocrPrefix + ptr->getValue<std::string>(ProductItemKey::is_ocr_equal_code).c_str() +
                                     separator + ptr->getValue<std::string>(ProductItemKey::logistics).c_str() + suffix;
            if (!PathNormal.isEmpty())
            {
                Utils::saveImageToLocal(location, PathNormal + loc_result);
                Utils::saveImageToLocal(check, PathNormal + check_result);
                Utils::saveImageToLocal(ocr, PathNormal + ocr_result);
            }
            if (ptr->isRemove())
            {
                const auto PathNG = AppSourceExtension::getFilePath(ResourcePath::PathImgNG, idNumber);
                if (!PathNG.isEmpty())
                {
                    Utils::saveImageToLocal(location, PathNG + loc_result);
                    Utils::saveImageToLocal(check, PathNG + check_result);
                    Utils::saveImageToLocal(ocr, PathNG + ocr_result);
                }
            }
        }
    });
}

void AppFrame::AppProductionExtension::afterCognexRecv(uint32_t number, const std::string &code)
{
    Utils::asyncTask([code, number] {
        // 二维码触发后处理
        const auto &product = appFramework().getPlugin<BaseProduct>();
        const auto &permission = appFramework().getPlugin<Permission>();
        const auto &plc = appFramework().getPlugin<BasePLCDevice>();
        // LogInfo("product id check,afterCognexRecv count = {}", number);

        auto currentProduct = product->updateValue(number, ProductItemKey::qr_code, code);
        if (currentProduct == nullptr)
        {
            // 无需再处理，仅界面通知并日志记录即可
            Json::Value jsReport;
            jsReport["diagText"] = "二维码触发错误，请查看！";
            LogError("product id={} updateValue error", number);
            appFramework().invokeCpp(&appMetaFlash(), "runtimeRoutine", PageIndex::PageHome,
                                     QString::fromStdString(Utils::makeResponse(true, std::move(jsReport), "error")));
            if (product->getType() != TypeProduct::TypeCircle)
            {
                // plc->writeBitValue(64, 120, 2, 1);
            }
            return;
        }
#ifdef APP_TEST
        auto [retNumber, retCode] = appTest().test_getLogistics(currentProduct->bottleNumber(), code);
        LogInfo("product id={},bottomNum={} afterCognexRecv", number, retNumber);
        if (number != retNumber)
        {
            LogError("product id={},bottomNum={} afterCognexRecv count do not equal retNumber", number, retNumber);
        }
        afterPermissionRecv(retNumber, retCode);
#endif
        appFramework().invokeCpp(permission, "sendQRCode", currentProduct->bottleNumber(), code);
    });
}

void AppFrame::AppProductionExtension::afterPermissionRecv(uint32_t number, const std::string &code)
{
    Utils::asyncTask([number, code] {
        LogInfo("afterPermissionRecv: {}, {}", number, code);
        const auto &product = appFramework().getPlugin<BaseProduct>();
        const auto &plcDev = appFramework().getPlugin<BasePLCDevice>();
        const auto &printerDevice = appFramework().getPlugin<PrinterDevice>();
        auto currentProduct = product->updateValue(number, ProductItemKey::logistics, code);
        if (currentProduct == nullptr)
        {
            // 仅记录日志
            LogError("afterPermissionRecv currentProduct nullptr");
            return;
        }
        // 直线式执行的打码逻辑
        if (product->getType() == TypeProduct::TypeLine)
        {
            if (code.size() == 24)
            {
                appFramework().invokeCpp(printerDevice, "pushCode", code);
            }

            plcDev->writeArray<uint8_t>(64, 142,
                                        product->getQueueToPLC<uint8_t>(TypeQueueToPLC::Print, currentProduct));
        }
    });
}

void AppFrame::AppProductionExtension::afterCaputureImage(const std::string windName, uint32_t number,
                                                          const cv::Mat &mat)
{
    Utils::asyncTask([windName, mat, number] {
        const auto &product = appFramework().getPlugin<BaseProduct>();
        const auto &webManager = appFramework().getPlugin<WebManager>();
        const auto &plcDev = appFramework().getPlugin<BasePLCDevice>();
        const auto &cameraManager = appFramework().getPlugin<CameraManager>();
        const auto &engine = appFramework().getEngine();
        const auto &plc = appFramework().getPlugin<BasePLCDevice>();
        appFramework().invokeCpp(engine->rootObjects().first()->findChild<QObject *>(windName.c_str()), "updateImage",
                                 Utils::matToQImage(mat));
        LogInfo("product id={},type={} afterCaputureImage", number, windName);
        std::string productKey;
        cv::Mat newImage;
        if (windName == "Location")
        {
            cv::resize(mat, newImage, {800, 800});
            productKey = ProductItemKey::location_path;
        }
        else if (windName == "OCR")
        {
            newImage = mat;
            productKey = ProductItemKey::ocr_path;
        }
        else if (windName == "LocateCheck")
        {
            newImage = mat;
            productKey = ProductItemKey::check_path;
        }
        auto currentProduct = product->updateValue(number, productKey, ProductImgaeData{"1", newImage});
        if (currentProduct == nullptr)
        {
            // 无需再处理，仅界面通知并日志记录即可
            Json::Value jsReport;
            jsReport["diagText"] = "相机错位，请检查！";
            LogError("product id={},windName={},updateValue error", number, windName);
            appFramework().invokeCpp(&appMetaFlash(), "runtimeRoutine", PageIndex::PageHome,
                                     QString::fromStdString(Utils::makeResponse(true, std::move(jsReport), "error")));
            if (product->getType() != TypeProduct::TypeCircle)
            {
                // plc->writeBitValue(64, 120, 2, 1);
            }
            return;
        }
        std::string sendJson;
        QByteArray sendBytes;
        Utils::makeJsonAndByteArray(newImage, currentProduct->bottleNumber(), sendJson, sendBytes);
        appFramework().invokeCpp(webManager, "sendToALGO", windName, sendJson, sendBytes);
    });
}

void AppFrame::AppProductionExtension::afterALGORecv(const ALGOType modelType, const uint32_t bottomNum,
                                                     const std::string &result, const std::string &area)
{
    Utils::asyncTask([modelType, bottomNum, result, area] {
        // 修改后的number计数不可能再为0，所以出现0必然是算法 json解析异常，没有获取到正确的序号
        if (bottomNum == 0)
        {
            LogError("afterALGORecv json error");
            return;
        }
        std::string imageKey, drawText, windType;
        const auto &product = appFramework().getPlugin<BaseProduct>();
        const auto &plcDev = appFramework().getPlugin<BasePLCDevice>();
        const auto &cameraManager = appFramework().getPlugin<CameraManager>();
        const auto &engine = appFramework().getEngine();
        float num = removeNum;
        plcDev->writeSingle<float>(64, 4, num);
        removeNum = removeNum + 1;
        std::shared_ptr<ProductItem> currentProduct{nullptr};
        ALGOType algoModel = static_cast<ALGOType>(modelType);
        switch (algoModel)
        {
        case ALGOType::Tangle: {
            // 方瓶只有 0° 和 180°
            std::string tangleRes =
                tangleCount(result, product->getBottleShape(), product->getSubType(), product->getOffset());
            currentProduct = product->updateValue(bottomNum, ProductItemKey::location_result, tangleRes);
            if (currentProduct == nullptr)
            {
                // 算法返回与触发无关，无需处理，仅记录即可
                LogError("afterALGORecv ALGOType::Tangle currentProduct nullptr");
                return;
            }
            // 下发至旋转队列
            plcDev->writeArray<uint16_t>(0, 13005,
                                         product->getQueueToPLC<uint16_t>(TypeQueueToPLC::Rotating, currentProduct));
            imageKey = ProductItemKey::location_path;
            drawText = "tangle: " + tangleRes;
            windType = "Location";
            break;
        }
        case ALGOType::OCR: {
            currentProduct = product->updateValue(bottomNum, ProductItemKey::ocr_result, result);
            if (currentProduct == nullptr)
            {
                LogInfo("afterALGORecv ALGOType::OCR currentProduct nullptr");
                return;
            }
            if (product->getType() != TypeProduct::TypeCircle)
            {
                std::vector<uint8_t> removeQueue =
                    product->getQueueToPLC<uint8_t>(TypeQueueToPLC::Remove, currentProduct);
                plcDev->writeArray<uint8_t>(64, 136, removeQueue);
            }
            else
            {
                plcDev->writeArray<uint16_t>(0, 13039,
                                             product->getQueueToPLC<uint16_t>(TypeQueueToPLC::Remove, currentProduct));
                AppProductionExtension::afterProductOut(1); // 产品退出
            }
            imageKey = ProductItemKey::ocr_path;
            drawText = result;
            windType = "OCR";
            break;
        }
        case ALGOType::Check: {
            currentProduct = product->updateValue(bottomNum, ProductItemKey::check_result, result);
            if (currentProduct == nullptr)
            {
                LogError("afterALGORecv ALGOType::Check currentProduct nullptr");
                return;
            }
            // 下发至打码队列
            plcDev->writeArray<uint16_t>(0, 13037,
                                         product->getQueueToPLC<uint16_t>(TypeQueueToPLC::Print, currentProduct));
            if (result == "1")
            {
                const auto &printerDevice = appFramework().getPlugin<PrinterDevice>();
                auto curLogistics = currentProduct->getValue<std::string>(ProductItemKey::logistics);
                if (curLogistics.size() == 24)
                {
                    appFramework().invokeCpp(printerDevice, "pushCode", curLogistics);
                }
            }
            imageKey = ProductItemKey::check_path;
            drawText = result == "1" ? "定位复核成功" : "定位复核失败";
            windType = "LocateCheck";
            break;
        }
        default:
            break;
        }
        LogInfo("modelType: {}, imageKey: {}", windType, imageKey);
        auto mat = currentProduct->getValue<ProductImgaeData>(imageKey).image_;
        QImage drawImage = Utils::matToQImage(mat);
        Utils::drawImgText(drawImage, drawText, area);
        appFramework().invokeCpp(engine->rootObjects().first()->findChild<QObject *>(windType.c_str()), "updateImage",
                                 drawImage);
    });
}

std::string AppFrame::AppProductionExtension::tangleCount(const std::string &result, const BottleShape bottleShape,
                                                          const SubType subType, const int tangelOffset)
{
    int tangleValue = std::stoi(result);
    std::string tangleRes;
    if (bottleShape == BottleShape::Square)
    {
        if (tangleValue > 150) // 顺时针机器转90°，逆时针转-90
        {
            tangleValue = subType == SubType::ClockWise ? 90 : -90;
        }
        else // 顺时针机器转-90°，逆时针转90
        {
            tangleValue = subType == SubType::ClockWise ? -90 : 90;
        }
    }
    tangleValue = std::fmod(tangleValue + tangelOffset, 360.0f);
    tangleRes = std::to_string(tangleValue);
    return tangleRes;
}