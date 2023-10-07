#include "CircleDevice.h"
#include "AlertWapper.h"
#include <QDebug>
#include <bitset>
CircleDevice::CircleDevice(QObject *parent) : BasePLCDevice(parent)
{
    devType = DeviceType::CircleDevice;
}

CircleDevice::~CircleDevice()
{
}

void CircleDevice::parsingReadInfo(const uint16_t *readInfo, uint16_t size)
{
    std::map<std::string, std::string> mapRealAlertInfo;
    // 12289 ~ 12310
    const uint16_t baseAddress = readBeginAddress_ + 1; // 预先计算起始地址

    for (uint16_t i = 0; i < 22; i++)
    {
        if (readInfo[i] > 0)
        {
            std::bitset<16> temp(readInfo[i]);

            for (uint8_t j = 0; j < 16; j++)
            {
                if (temp.test(j))
                {
                    // 计算 key
                    const std::string key = fmt::format("4{}_{}", baseAddress + i, j);
                    // std::cout << "PLC Address = " << key << std::endl;

                    auto finder = regWapper_.mapAlertInfo.find(key);
                    if (finder != regWapper_.mapAlertInfo.end())
                    {
                        mapRealAlertInfo[key] = finder->second;
                    }
                }
            }
        }
    }

    AlertWapper::updateRealtimeAlert(mapRealAlertInfo);
}

void CircleDevice::parsingRealtimeInfo(const uint16_t *realtimeInfo, uint16_t size)
{
    fifoInfo_.numPosition = realtimeInfo[2];
    fifoInfo_.numVerifyPos = realtimeInfo[3];
    fifoInfo_.numCoding = realtimeInfo[4];
    fifoInfo_.numVerifyCoding = realtimeInfo[5];
    fifoInfo_.signalMove = realtimeInfo[12];
    if (realtimeInfo[1] != fifoInfo_.numQRCode)
    {
        fifoInfo_.numQRCode = realtimeInfo[1];
        emit signalQR(fifoInfo_.numQRCode);
    }
}

void CircleDevice::updateReadInfo()
{
    AlertWapper::modifyAllStatus();
    BasePLCDevice::updateReadInfo();
}
