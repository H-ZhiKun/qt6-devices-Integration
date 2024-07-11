#pragma once
#include "AppMetaFlash.h"
#include "AppQmlExtension.h"
#include "BasePLCDevice.h"
#include "BaseProduct.h"
#include "BaseStatistics.h"
#include "Utils.h"
#include <AppFramework.h>
#include <cmath>
#include <vector>

namespace AppFrame
{
enum class AppPLCCountType
{
    amount = static_cast<int>(StatisticsKey::amount),
    code_trigger = static_cast<int>(StatisticsKey::code_trigger),
    code_get = static_cast<int>(StatisticsKey::code_get),
    print_send = static_cast<int>(StatisticsKey::print_send),
    print_right = static_cast<int>(StatisticsKey::print_right),
    ocr_trigger = static_cast<int>(StatisticsKey::ocr_trigger),
    ocr_get = static_cast<int>(StatisticsKey::ocr_get),
    locate_trigger = static_cast<int>(StatisticsKey::locate_trigger),
    locate_get = static_cast<int>(StatisticsKey::locate_get),
    check_trigger = static_cast<int>(StatisticsKey::check_trigger),
    check_get = static_cast<int>(StatisticsKey::check_get),
};

class AppStatisticsExtension : public NonCopyable
{
  public:
    static void afterProductCount(const std::shared_ptr<ProductItem> &pItem)
    {
        appFramework().runAfter("afterProductCount", [pItem] {
            const auto &stat = appFramework().getPlugin<BaseStatistics>();
            auto qrcode = pItem->getValue<std::string>(ProductItemKey::qr_code);
            auto logistics = pItem->getValue<std::string>(ProductItemKey::logistics);
            const auto &product = appFramework().getPlugin<BaseProduct>();
            std::vector<ReportKey> repordData;
            repordData.push_back(ReportKey::amount);
            if (qrcode == "no read")
            {
                stat->updateCount(StatisticsKey::code_error);
                repordData.push_back(ReportKey::qrcodeFaile);
            }
            else
            {
                stat->updateCount(StatisticsKey::code_right);
            }
            if (product->getType() == TypeProduct::TypeCircle)
            {
                float checkAnalysis = std::stof(pItem->getValue<std::string>(ProductItemKey::check_result));
                if (checkAnalysis <= 8 && checkAnalysis >= -8)
                {
                    stat->updateCount(StatisticsKey::check_perfect);
                    stat->updateCount(StatisticsKey::check_right);
                }
                else if (checkAnalysis <= 12 && checkAnalysis >= -12)
                {
                    stat->updateCount(StatisticsKey::check_good);
                    stat->updateCount(StatisticsKey::check_right);
                }
                else if (checkAnalysis <= 16 && checkAnalysis >= -16)
                {
                    stat->updateCount(StatisticsKey::check_commonly);
                    stat->updateCount(StatisticsKey::check_right);
                }
                else if (checkAnalysis < 360)
                {
                    stat->updateCount(StatisticsKey::check_bad);
                    stat->updateCount(StatisticsKey::check_right);
                }
                else
                {
                    stat->updateCount(StatisticsKey::check_error);
                    repordData.push_back(ReportKey::checkFaile);
                }
            }
            if (logistics.find("Eor1") != std::string::npos)
            {
                stat->updateCount(StatisticsKey::log_error);
                repordData.push_back(ReportKey::exceptionCode);
            }
            if (logistics.find("Eor2") != std::string::npos)
            {
                stat->updateCount(StatisticsKey::log_repeat);
                repordData.push_back(ReportKey::duplicateCode);
            }
            stat->updateCount(StatisticsKey::log_feedback);
            auto ocrRes = pItem->getValue<std::string>(ProductItemKey::is_ocr_equal_code);
            if (ocrRes == "true")
            {
                stat->updateCount(StatisticsKey::ocr_right);
            }
            else
            {
                stat->updateCount(StatisticsKey::ocr_error);
                repordData.push_back(ReportKey::ocrFaile);
                repordData.push_back(ReportKey::noInner);
            }
            if (logistics.find("Eor1") != std::string::npos || logistics.find("Eor2") != std::string::npos)
            {
                if (ocrRes == "false")
                    stat->updateCount(StatisticsKey::all_ng);
            }
            if (pItem->isRemove())
            {
                LogInfo("in StatisticsKey::remove");
                stat->updateCount(StatisticsKey::remove);
                repordData.push_back(ReportKey::faile);
            }
            else
            {
                LogInfo("in StatisticsKey::normal");
                stat->updateCount(StatisticsKey::normal);
            }
            if (stat->isContinuousNotGood())
            {
                const auto &plcDev = appFramework().getPlugin<BasePLCDevice>();
                const auto &product = appFramework().getPlugin<BaseProduct>();
                if (product->getType() == TypeProduct::TypeCap || product->getType() == TypeProduct::TypeLine ||
                    product->getType() == TypeProduct::TYpeBoxApp)
                {
                    plcDev->writeBitValue(64, 120, 3, 1);
                    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
                    plcDev->writeBitValue(64, 120, 3, 0);
                }
                else
                {
                    plcDev->writeBitValue(0, 12689, 2, 1);
                    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
                    plcDev->writeBitValue(0, 12689, 2, 0);
                }
                // 复位
                AppFrame::AppQmlExtension::zeroClearing();
                Json::Value jsReport;
                jsReport["diagText"] = "检查到连续NG，请查看！";
                appFramework().invokeCpp(
                    appFramework().getMeta(), "runtimeRoutine", PageIndex::PageHome,
                    QString::fromStdString(Utils::makeResponse(true, std::move(jsReport), "error")));
            }
        });
    }
    static void afterTriggerChanged(const AppPLCCountType type, uint32_t count = 1)
    {
        auto key = static_cast<StatisticsKey>(type);
        appFramework().runAfter("afterCountChanged", [key, count] {
            uint32_t tempCount = count;
            const auto &stat = appFramework().getPlugin<BaseStatistics>();
            while (tempCount > 0)
            {
                stat->updateCount(key);
                tempCount--;
            }
        });
    }
};
} // namespace AppFrame