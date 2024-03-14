#pragma once
#include "BaseStatistics.h"

class CapStatistics : public BaseStatistics
{
  public:
    explicit CapStatistics()
    {
        type_ = StatisticsType::CapType;
    }
    virtual ~CapStatistics() = default;
};
