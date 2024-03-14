#pragma once
#include "BasePLCDevice.h"

class CircleDevice : public BasePLCDevice
{
    Q_OBJECT
  public:
    explicit CircleDevice(QObject *parent = nullptr);
    virtual ~CircleDevice();

  protected:
    virtual bool afterInit() override;
    virtual void heartbeat() override;
    virtual void closeProductionLine() override;
    virtual void updateRealTimeInfo() override;
    virtual void parsingAlertInfo() override;
    virtual void parsingStatusInfo() override;
    virtual void parsingPLCIDInfo() override;
};
