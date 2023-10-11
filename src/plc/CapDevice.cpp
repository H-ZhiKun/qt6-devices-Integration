#include "CapDevice.h"
#include <QDebug>
#include <bitset>

CapDevice::CapDevice(QObject *parent) : BasePLCDevice(parent)
{
    devType = DeviceType::CapDevice;
}

CapDevice::~CapDevice()
{
}

void CapDevice::parsingReadInfo(const uint16_t *readInfo, uint16_t size)
{
}

void CapDevice::parsingRealtimeInfo(const uint16_t *realtimeInfo, uint16_t size)
{
    std::bitset<16> bit = std::bitset<16>(realtimeInfo[0]);
    // if (bit[8] == true && bit[8] != capInfo_.sigCoding)
    // {
    //     emit signalCoding();
    // }
    if (bit[9] == true && bit[9] != capInfo_.sigCognex)
    {
        emit signalQR(0);
    }
    if (bit[10] == true && bit[10] != capInfo_.sigOCR)
    {
        emit signalOCR();
    }
    if (bit[11] == true && bit[11] != capInfo_.sigRemove)
    {
        emit signalRemove();
    }
    // capInfo_.sigCoding = bit[8];
    capInfo_.sigCognex = bit[9];
    capInfo_.sigOCR = bit[10];
    capInfo_.sigRemove = bit[11];
}