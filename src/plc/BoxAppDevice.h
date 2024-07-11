#pragma once
#include "CapDevice.h"
#include <qtmetamacros.h>
#include <unordered_map>

class BoxAppDevice : public CapDevice
{
    Q_OBJECT
  public:
    explicit BoxAppDevice(QObject *parent = nullptr);
    virtual ~BoxAppDevice() = default;
    void idCompare(const uint32_t &currentId, const DeviceUnion key) override;
    bool getCollect(std::string windName) override;

  private:
    bool isCollect_ = false;
    short triggerNum = 0;
    std::mutex mtxCollect_;
    std::unordered_map<std::string, short> cameraTriggerNum_;
};