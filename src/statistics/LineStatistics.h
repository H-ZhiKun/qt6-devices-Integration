#pragma once
#include "BaseStatistics.h"

class LineStatistics : public BaseStatistics
{
  public:
    explicit LineStatistics()
    {
        type_ = StatisticsType::LineType;
    }
    virtual ~LineStatistics() = default;
};
