#pragma once
#include "BasePLCDevice.h"
#include "RegistersWapper.h"
struct FIFOInfo
{
    bool bhaveBottle = 0;            // 412642_0 是否有瓶
    uint16_t numQRCode = 0;          // 412643 二维码读取工位小瓶编号
    uint16_t numPosition = 0;        // 412644 定位拍照工位小瓶编号
    uint16_t numVerifyPos = 0;       // 412645 定位确认拍照工位小瓶编号
    uint16_t numCoding = 0;          // 412646 喷码工位小瓶编号
    uint16_t numVerifyCoding = 0;    // 412647 喷码复核工位小瓶编号
    uint16_t signalMove = 0;         // 412654 工位信号
    uint16_t signalSearchCoding = 0; // 412655 喷码数据查找信号
};
class CircleDevice : public BasePLCDevice
{
    Q_OBJECT
  public:
    explicit CircleDevice(QObject *parent = nullptr);
    virtual ~CircleDevice();

  protected:
    virtual void updateReadInfo() override;
    virtual void parsingReadInfo(const uint16_t *alertGroup, uint16_t size) override;
    virtual void parsingRealtimeInfo(const uint16_t *alertGroup, uint16_t size) override;

  private:
    RegistersWapper regWapper_;
    FIFOInfo fifoInfo_;
};