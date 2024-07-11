#pragma once
#include "CircleDevice.h"

class ImpurityDevice : public CircleDevice
{
    Q_OBJECT
  public:
    explicit ImpurityDevice(QObject *parent = nullptr);
    virtual ~ImpurityDevice() = default;
};