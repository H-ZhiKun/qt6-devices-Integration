#pragma once
#include "BasePLCDevice.h"

class CircleDevice : public BasePLCDevice
{
    Q_OBJECT
  public:
    explicit CircleDevice(QObject *parent = nullptr);
    virtual ~CircleDevice();
    virtual bool fetchFast() override; // 快取寄存器
    virtual bool fetchLazy() override; // 慢速寄存器
    virtual bool fetchTest() override; // 测试寄存器
  protected:
    virtual bool additionalInit() override;
    virtual bool clearArguments() override; // 设置启动参数初始化，阻塞调用
    virtual bool beforeClose() override;
    virtual void heart() override;
    virtual void parseFast() override;
    virtual void parseLazy() override;

    std::vector<uint16_t> fasts_; // 快速容器
    uint16_t fastAddress_ = 0;
    size_t fastSize_ = 0;
    std::vector<uint16_t> lazys_; // 慢速容器
    uint16_t lazyAddress_ = 0;
    size_t lazySize_ = 0;
};
