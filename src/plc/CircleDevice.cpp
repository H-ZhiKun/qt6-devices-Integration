#include "CircleDevice.h"
#include "AlertWapper.h"
#include <QDebug>
#include <bitset>
#include <cstdint>

CircleDevice::CircleDevice(QObject *parent) : BasePLCDevice(parent)
{
    devType = DeviceType::CircleDevice;
}

CircleDevice::~CircleDevice()
{
    closeProductionLine();
}

bool CircleDevice::afterInit()
{
    cacheAddress_ = 12289;
    caches_.resize(824);
    std::vector<uint8_t> vecInit(1);
    // 清零计数
    writeBitValue(0, 12992, 5, 1);
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    writeBitValue(0, 12992, 5, 0);
    if (!client_->readBytes(0, 12643, vecInit))
        return false;
    updateCaches(12643, vecInit);
    auto readDInts = readArray<uint32_t>(12643, 1);
    if (readDInts.size() > 0)
    {
        prudctInNumber_ = readDInts[0];
        return true;
    }
    return false;
}

void CircleDevice::updateRealTimeInfo()
{
    std::vector<uint8_t> tempId(1);
    std::vector<uint8_t> tempFull(824);
    auto lastAlertTime = std::chrono::high_resolution_clock::now();
    auto lastHeartTime = std::chrono::high_resolution_clock::now();
    while (updateHolder_.load(std::memory_order_acquire))
    {
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> durationAlert = currentTime - lastAlertTime;
        std::chrono::duration<double, std::milli> durationHeart = currentTime - lastHeartTime;
        if (client_ != nullptr && client_->readBytes(0, 12643, tempId))
        {
            updateCaches(12643, tempId);
            parsingPLCIDInfo();
        }
        if (durationAlert.count() > 1000.0f)
        {
            if (client_ != nullptr && client_->readBytes(0, 12289, tempFull))
            {
                updateCaches(12289, tempFull);
                parsingAlertInfo();
                parsingStatusInfo();
            }
            lastAlertTime = currentTime;
        }
        else if (durationHeart.count() > 3000.0f)
        {
            lastHeartTime = currentTime;
            heartbeat();
        }
        auto timeCost = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> consumption = timeCost - currentTime;
        if (consumption.count() > 50)
            qDebug() << "current plc RTT = " << consumption.count() << " milliseconds";
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
}

void CircleDevice::heartbeat()
{
    writeBitValue(0, 12992, 6, bHeartbeat_);
    bHeartbeat_ = (bHeartbeat_ + 1) % 2;
}

void CircleDevice::parsingAlertInfo()
{
    auto readWords = readArray<uint16_t>(12289, 23);
    if (readWords.size() > 0)
    {
        std::map<std::string, std::string> mapCurrentAlert;
        for (uint16_t i = 0; i < readWords.size(); i++)
        {
            if (readWords[i] > 0)
            {
                std::bitset<16> temp(readWords[i]);

                for (uint8_t j = 0; j < 16; j++)
                {
                    if (temp.test(j))
                    {
                        // 计算 key
                        const std::string key = fmt::format("0_{}_{}", 12289 + i, j);

                        auto finder = mapAlertStore_.find(key);
                        if (finder != mapAlertStore_.end())
                        {
                            qDebug() << "parsingAlertInfo key = " << key;
                            mapCurrentAlert[key] = finder->second;
                        }
                        else
                        {
                            qDebug() << "parsingAlertInfo key failed = " << key;
                        }
                    }
                }
            }
        }
        AlertWapper::updateRealtimeAlert(mapCurrentAlert, mapRecordAlert_);
    }
}

void CircleDevice::parsingStatusInfo()
{
    auto readWords = readArray<uint16_t>(12612, 2);
    if (readWords.size() > 0)
    {
        if (readWords[0] != plcStates_)
        {
            plcStates_ = readWords[0];
            emit signalStatus(plcStates_);
        }
        if (readWords[1] != plcSteps_)
        {
            // emit signalSteps(steps); 暂时没有用到此信号的触发
            plcSteps_ = readWords[1];
        }
    }
}

void CircleDevice::parsingPLCIDInfo()
{
    auto readWords = readArray<uint16_t>(12643, 1);
    if (readWords.size() > 0)
    {
        if (readWords[0] != prudctInNumber_)
        {
            if (readWords[0] > prudctInNumber_)
            {
                emit signalProductIn(readWords[0] - prudctInNumber_);
            }
            else
            {
                emit signalProductIn(readWords[0] - 0);
            }
            prudctInNumber_ = readWords[0];
        }
    }
}

void CircleDevice::closeProductionLine()
{
    writeBitValue(0, 12992, 2, 0);
}
