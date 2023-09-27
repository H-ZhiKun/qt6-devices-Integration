#include "LineDevice.h"
#include <QDebug>
#include <bitset>
LineDevice::LineDevice(QObject *parent) : BasePLCDevice(parent)
{
    devType = DeviceType::LineDevice;
}

void LineDevice::parsingReadInfo(const uint16_t *readInfo, uint16_t size)
{
}

void LineDevice::parsingRealtimeInfo(const uint16_t *realtimeInfo, uint16_t size)
{
    std::bitset<16> bit = std::bitset<16>(realtimeInfo[0]);
    if (bit[8] == true && bit[8] != lineInfo_.sigCoding)
    {
        emit signalCoding();
    }
    if (bit[9] == true && bit[9] != lineInfo_.sigCognex)
    {
        emit signalQR(0);
    }
    if (bit[10] == true && bit[10] != lineInfo_.sigOCR)
    {
        emit signalOCR();
    }
    if (bit[11] == true && bit[11] != lineInfo_.sigRemove)
    {
        emit signalRemove();
    }
    lineInfo_.sigCoding = bit[8];
    lineInfo_.sigCognex = bit[9];
    lineInfo_.sigOCR = bit[10];
    lineInfo_.sigRemove = bit[11];
}
