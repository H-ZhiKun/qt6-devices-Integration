#pragma once
#include "BasePLCDevice.h"

class CapDevice : public BasePLCDevice
{
    Q_OBJECT
  public:
    explicit CapDevice(QObject *parent = nullptr);
    virtual uint32_t getPlcCount(PLCCount) override;
    virtual ~CapDevice();

  protected:
    virtual bool afterInit() override;
    virtual void heartbeat() override;
    virtual void closeProductionLine() override;
    virtual void updateRealTimeInfo() override;
    virtual void parsingAlertInfo() override;
    virtual void parsingStatusInfo() override;
    virtual void parsingPLCIDInfo() override;
};