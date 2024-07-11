#pragma once
#include "BaseStatistics.h"

class CircleStatistics : public BaseStatistics
{
  public:
    explicit CircleStatistics();
    virtual ~CircleStatistics() = default;

  protected:
    virtual QVariantMap serialization(const std::unordered_map<StatisticsKey, int32_t> &tempMap) override;
};