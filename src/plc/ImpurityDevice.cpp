#include "ImpurityDevice.h"
ImpurityDevice::ImpurityDevice(QObject *parent) : CircleDevice(parent)
{
    devType = DeviceUnion::DevLine;
}