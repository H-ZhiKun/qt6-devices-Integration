#include "CapDevice.h"
#include "BasePLCDevice.h"
#include <Logger.h>
#include <cstdint>
#include <qtmetamacros.h>
#include <vector>
CapDevice::CapDevice(QObject *parent) : BasePLCDevice(parent)
{
    devType = DeviceUnion::DevCap;
}

CapDevice::~CapDevice()
{
}

bool CapDevice::additionalInit()
{
    dbNumber_ = 2;
    cacheAddress_ = 0;
    cacheSize_ = 310;
    fastAddress_ = 152;
    fastSize_ = 20;
    lazyAddress_ = 0;
    lazySize_ = 116;
    testAddress_ = 172;
    testSize_ = 138;
    caches_.resize(cacheSize_);
    fasts_.resize(fastSize_);
    lazys_.resize(lazySize_);
    tests_.resize(testSize_);

    // writeBitValue(64, 120, 2, 0);
    // // 清零计数
    // writeBitValue(64, 140, 0, 1);

    // std::vector<uint8_t> vecInit(20);
    // if (!client_->readBytes(2, 152, vecInit))
    //     return false;
    // updateCaches(152, vecInit);
    // auto readDInts = readArray<uint32_t>(152, 5);
    // if (readDInts.size() > 0 && readDInts[0] == 0)
    // {
    //     prudctInNumber_ = readDInts[0];
    //     productCameraNumber_ = readDInts[2];
    //     prudctOutNumber_ = readDInts[3];
    //     writeBitValue(64, 140, 0, 0);
    //     return true;
    // }
    return true;
}

bool CapDevice::fetchLazy()
{
    if (client_ != nullptr && client_->readBytes(dbNumber_, lazyAddress_, lazys_))
    {
        updateCaches(lazyAddress_, lazys_);
        parseLazy();
        return true;
    }
    else
    {
        return false;
    }
}

bool CapDevice::fetchFast()
{
    if (!bReady_)
    {
        return clearArguments();
    }
    auto fetchTime0 = std::chrono::high_resolution_clock::now();
    if (client_ != nullptr && client_->readBytes(dbNumber_, fastAddress_, fasts_))
    {
        updateCaches(fastAddress_, fasts_);
        parseFast();
        auto fetchTime1 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> consumption = fetchTime1 - fetchTime0;
        if (consumption.count() > 50)
            qDebug() << "PLC fetch RTT = " << consumption.count() << " milliseconds";
        return true;
    }
    else
    {
        return false;
    }
}

bool CapDevice::fetchTest()
{
    if (isTesting && client_ != nullptr && client_->readBytes(dbNumber_, testAddress_, tests_))
    {
        updateCaches(testAddress_, tests_);
        return true;
    }
    else
    {
        return false;
    }
}

bool CapDevice::clearArguments()
{
    bReady_ = true;
    return true;
}
void CapDevice::heart()
{
    if (writeBitValue(64, 134, 3, heartbeat_))
    {
        heartbeat_ = (heartbeat_ + 1) % 2;
    }
    else
    {
        LogError("heartbeat error.");
    }
}

void CapDevice::parseLazy()
{
    auto readBytes = readArray<uint8_t>(0, 11);
    if (readBytes.size() == 11)
    {
        parseAlert(0, readBytes);
    }
    auto readWords = readArray<uint16_t>(112, 2);
    if (readWords.size() == 2)
    {
        stateCompare(readWords[0], DeviceUnion::State);
        stateCompare(readWords[1], DeviceUnion::Step);
    }
}

void CapDevice::parseFast()
{
    auto readDWords = readArray<uint32_t>(fastAddress_, 5);
    if (readDWords.size() == 5)
    {
        idCompare(readDWords[0], DeviceUnion::In);
        idCompare(readDWords[1], DeviceUnion::Print);
        idCompare(readDWords[2], DeviceUnion::Ocr);
        idCompare(readDWords[3], DeviceUnion::Out);
        idCompare(readDWords[4], DeviceUnion::Qrcode);
    }
}