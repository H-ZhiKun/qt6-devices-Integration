#include "AppProductExtension.h"
#include "AppFramework.h"
#include "AppMetaFlash.h"
#include "AppSourceExtension.h"
#include "AppStatisticsExtension.h"
#include "BasePLCDevice.h"
#include "BaseProduct.h"
#include "CameraManager.h"
#include "ExtraTest.h"
#include "Logger.h"
#include "Permission.h"
#include "PrinterDevice.h"
#include "Utils.h"
#include "WebManager.h"
#include <cstdint>
#include <json/value.h>
#include <qimage.h>
#include <qtypes.h>
#include <string>

void AppFrame::AppProductionExtension::afterProductIn(const uint32_t count)
{
    appFramework().runAfter("afterProductIn", [count] {
        const auto &product = appFramework().getPlugin<BaseProduct>();
        if (product == nullptr)
            return;
        product->createProduct(count);
    });
}
void AppFrame::AppProductionExtension::afterProductOut(const uint32_t count)
{
    appFramework().runAfter("afterProductOut", [count] {
        const auto &product = appFramework().getPlugin<BaseProduct>();
        if (product == nullptr)
            return;
        auto delProducts = product->deleteProduct(count);

        const std::string locationPrefix = std::string("_location_");
        const std::string checkPrefix = std::string("_check_");
        const std::string ocrPrefix = std::string("_ocr_");
        const std::string suffix = std::string(".jpg");
        const std::string separator = std::string("_");

        for (auto ptr : delProducts)
        {
            const std::string idNumber = std::to_string(ptr->bottleNumber());

            const auto location = ptr->getValue<ProductImgaeData>(ProductItemKey::location_path).image_;
            const auto loc_result =
                idNumber + locationPrefix + ptr->getValue<std::string>(ProductItemKey::location_result) + suffix;

            const auto check = ptr->getValue<ProductImgaeData>(ProductItemKey::check_path).image_;
            const auto check_result =
                idNumber + checkPrefix + ptr->getValue<std::string>(ProductItemKey::check_result) + suffix;

            const auto ocr = ptr->getValue<ProductImgaeData>(ProductItemKey::ocr_path).image_;
            const auto ocr_result = idNumber + ocrPrefix +
                                    ptr->getValue<std::string>(ProductItemKey::is_ocr_equal_code) + separator +
                                    ptr->getValue<std::string>(ProductItemKey::logistics) + suffix;
            AppStatisticsExtension::afterProductCount(ptr);
            if (ptr->isRemove())
            {
                const auto PathNG = AppSourceExtension::getFilePath(ResourcePath::PathImgNG).toStdString();
                if (!PathNG.empty())
                {
                    Utils::saveMatToLocal(location, PathNG + loc_result);
                    Utils::saveMatToLocal(check, PathNG + check_result);
                    Utils::saveMatToLocal(ocr, PathNG + ocr_result);
                }
            }
            else
            {
                const auto PathNormal = AppSourceExtension::getFilePath(ResourcePath::PathImgNormal).toStdString();
                if (!PathNormal.empty())
                {
                    Utils::saveMatToLocal(location, PathNormal + loc_result);
                    Utils::saveMatToLocal(check, PathNormal + check_result);
                    Utils::saveMatToLocal(ocr, PathNormal + ocr_result);
                }
            }
        }
    });
}

void AppFrame::AppProductionExtension::afterCognexRecv(uint32_t number, const std::string &code)
{
    appFramework().runAfter("afterCognexRecv", [code, number] {
        // 二维码触发后处理
        const auto &product = appFramework().getPlugin<BaseProduct>();
        const auto &permission = appFramework().getPlugin<Permission>();
        const auto &plc = appFramework().getPlugin<BasePLCDevice>();
        const auto &test = appFramework().getPlugin<ExtraTest>();
        if (product == nullptr || permission == nullptr || plc == nullptr || test == nullptr)
            return;
        auto currentProduct = product->updateValue(number, ProductItemKey::qr_code, code);
        if (currentProduct == nullptr)
        {
            // 无需再处理，仅界面通知并日志记录即可
            Json::Value jsReport;
            jsReport["diagText"] = "二维码触发错误，请查看！";
            LogError("product id={} updateValue error", number);
            appFramework().invokeCpp(appFramework().getMeta(), "runtimeRoutine", PageIndex::PageHome,
                                     QString::fromStdString(Utils::makeResponse(true, std::move(jsReport), "error")));
            if (product->getType() != TypeProduct::TypeCircle)
            {
                // plc->writeBitValue(64, 120, 2, 1);
            }
            return;
        }

        if (test->isPrintTest())
        {
            auto [retNumber, retCode] = test->testLogistics(currentProduct->bottleNumber(), code);
            afterPermissionRecv(retNumber, retCode);
        }
        appFramework().invokeCpp(permission, "sendQRCode", currentProduct->bottleNumber(), code);
    });
}

void AppFrame::AppProductionExtension::afterPermissionRecv(uint32_t number, const std::string &code)
{
    appFramework().runAfter("afterPermissionRecv", [number, code] {
        const auto &test = appFramework().getPlugin<ExtraTest>();
        const auto &product = appFramework().getPlugin<BaseProduct>();
        const auto &plcDev = appFramework().getPlugin<BasePLCDevice>();
        const auto &printerDevice = appFramework().getPlugin<PrinterDevice>();
        if (test == nullptr || product == nullptr || plcDev == nullptr || printerDevice == nullptr)
        {
            return;
        }
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
            appFramework().invokeCpp(printerDevice, "pushCode", code);
            LogInfo("push code: {}", code);
            plcDev->writeArray<uint8_t>(
                64, 142,
                product->getQueueToPLC<uint8_t>(TypeQueueToPLC::Print, currentProduct->isPrint(), currentProduct));
        }
    });
}

void AppFrame::AppProductionExtension::afterGetImage(const std::string windName, uint32_t number, cv::Mat &&mat)
{
    appFramework().runAfter("afterCaputureImage", [windName, mat, number] {
        const auto &product = appFramework().getPlugin<BaseProduct>();
        const auto &webManager = appFramework().getPlugin<WebManager>();
        const auto &plcDev = appFramework().getPlugin<BasePLCDevice>();
        const auto &cameraManager = appFramework().getPlugin<CameraManager>();
        const auto &engine = appFramework().getEngine();
        if (product == nullptr || webManager == nullptr || plcDev == nullptr || cameraManager == nullptr ||
            engine == nullptr)
        {
            return;
        }
        appFramework().invokeCpp(engine->rootObjects().first()->findChild<QObject *>(windName.c_str()), "updateImage",
                                 Utils::matToQImage(mat));
        LogInfo("product id={},type={} afterCaputureImage", number, windName);
        std::string productKey;
        cv::Mat newImage;
        if (windName == "Location")
        {
            cv::resize(mat, newImage, {800, 800});
            productKey = ProductItemKey::location_path;
            AppStatisticsExtension::afterTriggerChanged(AppPLCCountType::locate_get);
        }
        else if (windName == "OCR")
        {
            cv::resize(mat, newImage, {1024, 1024});
            productKey = ProductItemKey::ocr_path;
            AppStatisticsExtension::afterTriggerChanged(AppPLCCountType::ocr_get);
        }
        else if (windName == "LocateCheck")
        {
            newImage = mat;
            productKey = ProductItemKey::check_path;
            AppStatisticsExtension::afterTriggerChanged(AppPLCCountType::check_get);
        }
        else
        {
            newImage = mat;
            productKey = ProductItemKey::ocr_path;
        }
        auto currentProduct = product->updateValue(number, productKey, ProductImgaeData{"1", newImage});
        if (currentProduct == nullptr)
        {
            // 无需再处理，仅界面通知并日志记录即可
            Json::Value jsReport;
            jsReport["diagText"] = "相机错位，请检查！";
            LogError("product id={},windName={},updateValue error", number, windName);
            appFramework().invokeCpp(appFramework().getMeta(), "runtimeRoutine", PageIndex::PageHome,
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
    appFramework().runAfter("afterALGORecv", [modelType, bottomNum, result, area] {
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
        const auto &test = appFramework().getPlugin<ExtraTest>();
        const auto &engine = appFramework().getEngine();
        if (product == nullptr || plcDev == nullptr || cameraManager == nullptr || test == nullptr || engine == nullptr)
        {
            return;
        }
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
            plcDev->writeArray<uint16_t>(
                0, 13005,
                product->getQueueToPLC<uint16_t>(TypeQueueToPLC::Rotating,
                                                 currentProduct->getValue<std::string>(ProductItemKey::location_result),
                                                 currentProduct));
            imageKey = ProductItemKey::location_path;
            LogInfo("Tangle send to plc: {}", tangleRes);
            drawText = "tangle: " + tangleRes;
            windType = "Location";
            break;
        }
        case ALGOType::OCR: {
            currentProduct = product->updateValue(bottomNum, ProductItemKey::ocr_result, result);
            if (currentProduct == nullptr)
            {
                LogInfo("afterALGORecv ALGOType::OCR currentProduct nullptr, bottomNum = {}", bottomNum);
                return;
            }
            if (product->getType() == TypeProduct::TypeCap)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(300));
            }
            bool needRemove = false;
            if (test->isTestRemove())
            {
                needRemove = test->getPosRemoveNeeded();
            }
            else
            {
                needRemove = currentProduct->isRemove();
            }
            if (product->getType() == TypeProduct::TypeCap || product->getType() == TypeProduct::TypeLine)
            {
                std::vector<uint8_t> removeQueue =
                    product->getQueueToPLC<uint8_t>(TypeQueueToPLC::Remove, needRemove, currentProduct);
                plcDev->writeArray<uint8_t>(64, 136, removeQueue);
            }
            else
            {
                plcDev->writeArray<uint16_t>(
                    0, 13039, product->getQueueToPLC<uint16_t>(TypeQueueToPLC::Remove, needRemove, currentProduct));
                AppProductionExtension::afterProductOut(1); // 产品退出
            }
            imageKey = ProductItemKey::ocr_path;
            drawText = result;
            windType = "OCR";
            break;
        }
        case ALGOType::Check: {
            /*if (product->getBottleShape() == BottleShape::Square)
            {
                if (result != "361")
                {
                    currentProduct = product->updateValue(bottomNum, ProductItemKey::check_result, result);
                }
                else
                {
                    currentProduct = product->updateValue(bottomNum, ProductItemKey::check_result, result);
                }
                currentProduct = product->updateValue(bottomNum, ProductItemKey::rerotate_result, "0");
            }*/
            /*if (product->getBottleShape() == BottleShape::Round)
            {*/
            int checkRes = std::stoi(result);
            std::string checkStr = result;
            if (checkRes < 1 && checkRes > -1)
            {
                checkRes = 0;
            }
            else if (checkRes == 361)
            {
                checkRes = 0;
            }
            else if (checkRes < 5)
            {
                checkRes += 360;
            }
            checkStr = std::to_string(checkRes);
            product->updateValue(bottomNum, ProductItemKey::check_result, result);
            currentProduct = product->updateValue(bottomNum, ProductItemKey::rerotate_result, std::to_string(checkRes));
            /*}
             */
            if (currentProduct == nullptr)
            {
                LogError("afterALGORecv ALGOType::Check currentProduct nullptr, product id = {}", bottomNum);
                return;
            }
            // 第二次旋转
            plcDev->writeArray<uint16_t>(
                0, 13049,
                product->getQueueToPLC<uint16_t>(TypeQueueToPLC::ReRotating,
                                                 currentProduct->getValue<std::string>(ProductItemKey::rerotate_result),
                                                 currentProduct));
            // 下发至打码队列
            plcDev->writeArray<uint16_t>(
                0, 13037,
                product->getQueueToPLC<uint16_t>(TypeQueueToPLC::Print, currentProduct->isPrint(), currentProduct));
            if (result != "361")
            {
                // 下发打码数据
                const auto &printerDevice = appFramework().getPlugin<PrinterDevice>();
                auto curLogistics = currentProduct->getValue<std::string>(ProductItemKey::logistics);
                if (curLogistics.size() == 24)
                {
                    appFramework().invokeCpp(printerDevice, "pushCode", curLogistics);
                }
                else
                {
                    LogError("push code error, logistics = {}", curLogistics);
                }
            }
            imageKey = ProductItemKey::check_path;
            drawText = result == "361" ? "定位复核失败" : "定位复核成功";
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