#include "CapDevice.h"
#include "AlertWapper.h"
#include "BasePLCDevice.h"
#include "Utils.h"
#include <Logger.h>
#include <bitset>
#include <cstdint>
#include <qtmetamacros.h>
#include <vector>
CapDevice::CapDevice(QObject *parent) : BasePLCDevice(parent)
{
    devType = DeviceType::CapDevice;
    bytesFlip_ = true;
}

CapDevice::~CapDevice()
{
    closeProductionLine();
}

bool CapDevice::afterInit()
{
    cacheAddress_ = 0;
    caches_.resize(310);
    writeBitValue(64, 120, 2, 0);
    // 清零计数
    writeBitValue(64, 140, 0, 1);

    std::vector<uint8_t> vecInit(20);
    if (!client_->readBytes(2, 152, vecInit))
        return false;
    updateCaches(152, vecInit);
    auto readDInts = readArray<uint32_t>(152, 5);
    if (readDInts.size() > 0 && readDInts[0] == 0)
    {
        prudctInNumber_ = readDInts[0];
        productCameraNumber_ = readDInts[2];
        prudctOutNumber_ = readDInts[3];
        writeBitValue(64, 140, 0, 0);
        return true;
    }
    return false;
}

void CapDevice::updateRealTimeInfo()
{
    std::vector<uint8_t> tempId(20);
    std::vector<uint8_t> tempAlert(116);
    std::vector<uint8_t> tempUnReal(138);
    auto lastAlertTime = std::chrono::high_resolution_clock::now();
    auto lastHeartTime = std::chrono::high_resolution_clock::now();
    auto lastUnRealTime = std::chrono::high_resolution_clock::now();
    while (updateHolder_.load(std::memory_order_acquire))
    {
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> durationAlert = currentTime - lastAlertTime;
        std::chrono::duration<double, std::milli> durationHeart = currentTime - lastHeartTime;
        std::chrono::duration<double, std::milli> durationtUnReal = currentTime - lastUnRealTime;
        if (client_ != nullptr && client_->readBytes(2, 152, tempId))
        {
            if (updateHolder_.load(std::memory_order_acquire) == false)
                return;
            updateCaches(152, tempId);
            parsingPLCIDInfo();
        }
        if (durationtUnReal.count() > 500.0f)
        {
            if (client_ != nullptr && client_->readBytes(2, 172, tempUnReal))
            {
                updateCaches(172, tempUnReal);
            }
            lastUnRealTime = currentTime;
        }
        else if (durationAlert.count() > 1000.0f)
        {
            if (client_ != nullptr && client_->readBytes(2, 0, tempAlert))
            {
                updateCaches(0, tempAlert);
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

void CapDevice::heartbeat()
{
    Utils::asyncTask([this] {
        writeBitValue(64, 134, 3, bHeartbeat_);
        bHeartbeat_ = (bHeartbeat_ + 1) % 2;
    });
}

void CapDevice::parsingAlertInfo()
{
    auto readBytes = readArray<uint8_t>(0, 11);
    if (readBytes.size() > 0)
    {
        std::map<std::string, std::string> mapCurrentAlert;
        for (uint16_t i = 0; i < 11; i++)
        {
            if (readBytes[i] > 0)
            {
                std::bitset<8> temp(readBytes[i]);

                for (uint8_t j = 0; j < 8; j++)
                {
                    if (temp.test(j))
                    {
                        // 计算 key
                        const std::string key = fmt::format("2_{}_{}", i, j);

                        auto finder = mapAlertStore_.find(key);
                        if (finder != mapAlertStore_.end())
                        {
                            // qDebug() << "parsingAlertInfo key = " << key;
                            mapCurrentAlert[key] = finder->second;
                        }
                    }
                }
            }
        }
        AlertWapper::updateRealtimeAlert(mapCurrentAlert, mapRecordAlert_);
    }
}

void CapDevice::parsingStatusInfo()
{
    auto readWords = readArray<uint16_t>(112, 2);
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

void CapDevice::parsingPLCIDInfo()
{
    auto readWords = readArray<uint32_t>(152, 5);
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
        if (readWords[3] != prudctOutNumber_)
        {
            if (readWords[3] > prudctOutNumber_)
            {
                emit signalProductOut(readWords[3] - prudctOutNumber_);
            }
            else
            {
                emit signalProductOut(readWords[3] - 0);
            }
            prudctOutNumber_ = readWords[3];
        }
        if (readWords[2] != productCameraNumber_)
        {
            emit signalOCR();
            productCameraNumber_ = readWords[2];
            LogInfo("product id={}, camera signal up", readWords[2]);
        }
        if (readWords[4] != productQRCodeNumber_)
        {
            emit signalQR();
            productQRCodeNumber_ = readWords[4];
            LogInfo("product id={}, qrCode signal up", readWords[4]);
        }
        productCodeNumber_ = readWords[1];
    }
}

void CapDevice::closeProductionLine()
{
}

uint32_t CapDevice::getPlcCount(PLCCount plcCount)
{
    switch (plcCount)
    {
    case PLCCount::InCount: {
        return prudctInNumber_;
    }
    case PLCCount::OutCount: {
        return prudctOutNumber_;
    }
    case PLCCount::CameraCount: {
        return productCameraNumber_;
    }
    case PLCCount::QRCodeCount: {
        return productQRCodeNumber_;
    }
    case PLCCount::CodeCount: {
        return productCodeNumber_;
    }
    default:
        break;
    }
    return 0;
}