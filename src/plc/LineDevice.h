#pragma once
#include "CapDevice.h"

class LineDevice : public CapDevice
{
    Q_OBJECT
  public:
    explicit LineDevice(QObject *parent = nullptr);
    virtual ~LineDevice() = default;
};
