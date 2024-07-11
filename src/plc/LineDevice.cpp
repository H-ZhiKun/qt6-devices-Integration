#include "LineDevice.h"
LineDevice::LineDevice(QObject *parent) : CapDevice(parent)
{
    devType = DeviceUnion::DevLine;
}