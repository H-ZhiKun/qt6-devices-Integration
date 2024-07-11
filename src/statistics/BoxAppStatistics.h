#pragma once
#include "BaseStatistics.h"

class BoxAppStatistics : public BaseStatistics
{
  public:
    explicit BoxAppStatistics();
    virtual ~BoxAppStatistics() = default;
    virtual void updateCount(const StatisticsKey key) override;

  protected:
    virtual QVariantMap serialization(const std::unordered_map<StatisticsKey, int32_t> &tempMap) override;
};