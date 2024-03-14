#pragma once
#include "BaseStatistics.h"
#include "Utils.h"
#include <AppFramework.h>

namespace AppFrame
{
enum class AppPLCCountType
{
    amount = 0,
    code_trigger,
    code_get,
    code_right,
    code_feedback,
    print_send,
    print_right,
    ocr_trigger,
    ocr_get,
    locate_trigger,
    locate_get,
    locate_right,
    check_trigger,
    check_get,
    check_right,
    remove
};

class AppStatisticsExtension : public NonCopyable
{
  public:
    static void afterCountChanged(const AppPLCCountType type)
    {
        auto key = static_cast<StatisticsKey>(type);
        Utils::asyncTask([key] {
            const auto &stat = appFramework().getPlugin<BaseStatistics>();
            stat->updateCount(key);
        });
    }
};
} // namespace AppFrame