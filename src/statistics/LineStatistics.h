#pragma once
#include "BaseStatistics.h"

class LineStatistics : public BaseStatistics
{
  public:
    explicit LineStatistics();
    virtual ~LineStatistics() = default;

  protected:
    virtual QVariantMap serialization(const std::unordered_map<StatisticsKey, int32_t> &tempMap) override;
};
