#include "BoxAppDevice.h"

BoxAppDevice::BoxAppDevice(QObject *parent) : CapDevice(parent)
{
    devType = DeviceUnion::DevLine;
    cameraTriggerNum_["range1"] = -1;
    cameraTriggerNum_["range2"] = -1;
    cameraTriggerNum_["range3"] = -1;
    cameraTriggerNum_["range4"] = -1;
    cameraTriggerNum_["range5"] = -1;
}

void BoxAppDevice::idCompare(const uint32_t &currentId, const DeviceUnion key)
{
    auto idFinder = mapDevDatas_.find(key);
    if (idFinder == mapDevDatas_.end())
        return;
    auto &prevId = idFinder->second;
    if (currentId != prevId)
    {
        if (currentId > prevId)
        {
            mapDevSignals_.at(key)(currentId - prevId);
            LogInfo("product id={},{} signal up", currentId, static_cast<uint8_t>(key));
        }
        if (key == DeviceUnion::Print)
        {
            emit triggerImage();
            LogInfo("trigger image");
        }
        prevId = currentId;
    }
}

bool BoxAppDevice::getCollect(std::string windName)
{
    std::lock_guard lock(mtxCollect_);
    if (cameraTriggerNum_[windName] == 5 || cameraTriggerNum_[windName] == -1)
    {
        cameraTriggerNum_[windName] = -1;
        return false;
    }
    ++cameraTriggerNum_[windName];
    return true;
}