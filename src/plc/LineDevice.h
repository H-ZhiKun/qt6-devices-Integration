#pragma once
#include "BasePLCDevice.h"
#include "RegistersWapper.h"

struct LineInfo
{
    bool sigCoding = false; // 40081.8 打码信号
    bool sigCognex = false; // 40081.9 读码信号
    bool sigOCR = false;    // 40081.10 OCR信号
    bool sigRemove = false; // 40081.11 剔除信号
};

class LineDevice : public BasePLCDevice
{
    Q_OBJECT
  public:
    explicit LineDevice(QObject *parent = nullptr);
    ~LineDevice() = default;

  protected:
    virtual void parsingReadInfo(const uint16_t *alertGroup, uint16_t size) override;
    virtual void parsingRealtimeInfo(const uint16_t *alertGroup, uint16_t size) override;

  private:
    LineInfo lineInfo_;
};