#pragma once
#include "BaseStatistics.h"
#include <AppFramework.h>

namespace AppFrame
{

class AppCentralExtension : public NonCopyable
{
  public:
    static void uploadData(const std::unordered_map<ReportKey, int32_t> data);
};
} // namespace AppFrame