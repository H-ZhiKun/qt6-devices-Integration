#pragma once
#include "BaseStatistics.h"

class ImpurityStatistics : public BaseStatistics
{
  public:
    explicit ImpurityStatistics();
    virtual ~ImpurityStatistics() = default;
    virtual void updateCount(const StatisticsKey key) override;

  protected:
    virtual QVariantMap serialization(const std::unordered_map<StatisticsKey, int32_t> &tempMap) override;
};