#include "CircleDevice.h"
#include <cstdint>

CircleDevice::CircleDevice(QObject *parent) : BasePLCDevice(parent)
{
    devType = DeviceUnion::DevCircle;
}

CircleDevice::~CircleDevice()
{
}

bool CircleDevice::additionalInit()
{
    cacheAddress_ = 12289;
    cacheSize_ = 824;
    fastAddress_ = 12643;
    fastSize_ = 4;
    lazyAddress_ = 12289;
    lazySize_ = 824;

    caches_.resize(cacheSize_);
    fasts_.resize(fastSize_);
    lazys_.resize(lazySize_);
    return true;
}
bool CircleDevice::clearArguments()
{
    auto ret1 = writeBitValue(0, 12992, 1, 1);
    auto ret2 = writeBitValue(0, 12992, 5, 1);
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    auto ret3 = writeBitValue(0, 12992, 1, 0);
    auto ret4 = writeBitValue(0, 12992, 5, 0);
    bReady_ = ret1 && ret2 && ret3 && ret4;
    return bReady_;
}
bool CircleDevice::beforeClose()
{
    clearArguments();
    return true;
}
bool CircleDevice::fetchLazy()
{
    if (client_ != nullptr && client_->readWords(dbNumber_, lazyAddress_, lazys_))
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

bool CircleDevice::fetchFast()
{
    if (!bReady_)
    {
        return clearArguments();
    }
    auto fetchTime0 = std::chrono::high_resolution_clock::now();
    if (client_ != nullptr && client_->readWords(dbNumber_, fastAddress_, fasts_))
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
bool CircleDevice::fetchTest()
{
    return true;
}
void CircleDevice::heart()
{
    if (writeBitValue(0, 12992, 6, heartbeat_))
    {
        heartbeat_ = (heartbeat_ + 1) % 2;
    }
    else
    {
        LogError("heartbeat error.");
    }
}

void CircleDevice::parseLazy()
{
    auto readAlert = readArray<uint16_t>(lazyAddress_, 23);
    if (readAlert.size() == 23)
    {
        parseAlert(lazyAddress_, readAlert);
    }
    auto readWords = readArray<uint16_t>(12612, 2);
    {
        stateCompare(readWords[0], DeviceUnion::State);
        stateCompare(readWords[1], DeviceUnion::Step);
    }
}

void CircleDevice::parseFast()
{
    auto readDWords = readArray<uint32_t>(fastAddress_, 4);
    if (readDWords.size() == 4)
    {
        idCompare(readDWords[0], DeviceUnion::In);
        idCompare(readDWords[1], DeviceUnion::Qrcode);
        idCompare(readDWords[2], DeviceUnion::Location);
        idCompare(readDWords[3], DeviceUnion::LocateCheck);
    }
}
