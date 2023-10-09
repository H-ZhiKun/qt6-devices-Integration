#pragma once
#include "BasePLCDevice.h"
#include "RegistersWapper.h"

struct CapInfo
{
    bool sigCoding = false; // 40080.8 打码信号
    bool sigCognex = false; // 40080.9 读码信号
    bool sigOCR = false;    // 40080.10 OCR信号
    bool sigRemove = false; // 40080.11 剔除信号
};

class CapDevice : public BasePLCDevice
{
    Q_OBJECT
  public:
    explicit CapDevice(QObject *parent = nullptr);
    virtual ~CapDevice();

  protected:
    virtual void parsingReadInfo(const uint16_t *alertGroup, uint16_t size) override;
    virtual void parsingRealtimeInfo(const uint16_t *alertGroup, uint16_t size) override;

  private:
    CapInfo capInfo_;
};