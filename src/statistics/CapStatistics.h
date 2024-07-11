#pragma once
#include "BaseStatistics.h"

class CapStatistics : public BaseStatistics
{
  public:
    explicit CapStatistics();
    virtual ~CapStatistics() = default;
    virtual void updateCount(const StatisticsKey key) override;

  protected:
    virtual QVariantMap serialization(const std::unordered_map<StatisticsKey, int32_t> &tempMap) override;
};
