#pragma once
#include "BasePLCDevice.h"

class CapDevice : public BasePLCDevice
{
    Q_OBJECT
  public:
    explicit CapDevice(QObject *parent = nullptr);
    virtual ~CapDevice();
    virtual bool fetchFast() override; // 快取寄存器
    virtual bool fetchLazy() override; // 慢速寄存器
    virtual bool fetchTest() override; // 测试寄存器
  protected:
    virtual bool additionalInit() override;
    virtual bool clearArguments() override; // 设置计数清零
    virtual void heart() override;
    virtual void parseFast() override;
    virtual void parseLazy() override;

    std::vector<uint8_t> fasts_; // 快速容器
    uint16_t fastAddress_ = 0;
    size_t fastSize_ = 0;
    std::vector<uint8_t> tests_; // 测试容器 （电机状态位置等非实时读寄存器）
    uint16_t testAddress_ = 0;
    size_t testSize_ = 0;
    std::vector<uint8_t> lazys_; // 慢速容器
    uint16_t lazyAddress_ = 0;
    size_t lazySize_ = 0;
};