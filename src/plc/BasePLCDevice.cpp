#include "BasePLCDevice.h"
#include "Utils.h"
#include <QDebug>
#include <bitset>
BasePLCDevice::BasePLCDevice(QObject *parent) : QObject(parent)
{
}

BasePLCDevice::~BasePLCDevice()
{
    updateHolder_.store(false, std::memory_order_release);
    thUpdate_.join();
    if (client_)
    {
        delete client_;
        client_ = nullptr;
    }
}

void BasePLCDevice::init(const YAML::Node &config)
{
    ModbusInitArguments args;

    args.ip = config["plc"]["host"].as<std::string>();
    args.port = config["plc"]["port"].as<uint16_t>();
    std::string normalAddress = config["plc"]["normal_address"].as<std::string>();
    std::string writeAddress = config["plc"]["write_address"].as<std::string>();
    std::string realtimeAddress = config["plc"]["realtime_address"].as<std::string>();
    readBeginAddress_ = Utils::anyFromString<uint16_t>(normalAddress.substr(1)) - 1;
    readCacheSize_ = config["plc"]["normal_size"].as<uint16_t>();
    writeBeginAddress_ = Utils::anyFromString<uint16_t>(writeAddress.substr(1)) - 1;
    writeCacheSize_ = config["plc"]["write_size"].as<uint16_t>();
    realtimeBeginAddress_ = Utils::anyFromString<uint16_t>(realtimeAddress.substr(1)) - 1;
    realtimeCacheSize_ = config["plc"]["realtime_size"].as<uint16_t>();

    client_ = new ModbusClient(std::move(args));
    client_->addWriteCache(writeBeginAddress_, writeCacheSize_);
    client_->addNormalCache(readBeginAddress_, readCacheSize_);
    client_->addRealtimeCache(realtimeBeginAddress_, realtimeCacheSize_);
    client_->work();
    updateReadInfo();
}

std::string BasePLCDevice::readDevice(const std::string &type, const std::string &addr, const std::string &bit)
{
    std::string ret;
    if (!addr.empty() && !type.empty())
    {
        std::vector<uint16_t> data(2);
        uint16_t plcAddr = Utils::anyFromString<uint16_t>(addr);
        if (client_->readCache(plcAddr, 2, data))
        {
            if (type == "b")
            {
                uint16_t bitPos = Utils::anyFromString<uint16_t>(bit);
                ret = std::to_string(std::bitset<16>(data[0])[bitPos]);
            }
            else if (type == "r")
            {
                uint32_t combinedValue = (static_cast<uint32_t>(data[1]) << 16) | static_cast<uint32_t>(data[0]);
                ret = fmt::format("{:.2f}", static_cast<float>(combinedValue) / 100.0);
            }
            else if (type == "n")
            {
                ret = std::to_string(data[0]);
            }
            else if (type == "di")
            {
                uint32_t combinedValue = static_cast<uint32_t>(data[1]) | (static_cast<uint32_t>(data[0]) << 16);
                ret = std::to_string(combinedValue);
            }
        }
    }
    return ret;
}

bool BasePLCDevice::writeDevice(const std::string &type, const std::string &addr, const std::string &bit,
                                const std::string &value)
{
    bool ret = false;
    if (!addr.empty() && !type.empty() && !value.empty())
    {
        uint16_t data[2] = {};
        uint16_t plcAddr = Utils::anyFromString<uint16_t>(addr);
        WriteRegisterType regType;
        if (type == "b")
        {
            data[0] = Utils::anyFromString<uint16_t>(bit);
            data[1] = Utils::anyFromString<uint16_t>(value);
            regType = WriteRegisterType::RegBool;
        }
        else if (type == "n")
        {
            plcAddr = Utils::anyFromString<uint16_t>(addr);
            data[0] = Utils::anyFromString<uint16_t>(value);
            regType = WriteRegisterType::RegInt;
        }
        else if (type == "r")
        {
            plcAddr = Utils::anyFromString<uint16_t>(addr);
            uint32_t uint32Val = Utils::anyFromString<float>(value) * 100;
            data[0] = (uint16_t)uint32Val;
            data[1] = (uint16_t)(uint32Val >> 16);
            regType = WriteRegisterType::RegReal;
        }
        else if (type == "di")
        {
            plcAddr = Utils::anyFromString<uint16_t>(addr);
            uint32_t uint32Val = Utils::anyFromString<uint32_t>(value);
            data[0] = (uint16_t)(uint32Val >> 16);
            data[1] = (uint16_t)uint32Val;
            regType = WriteRegisterType::RegDInt;
        }

        client_->writeCache(plcAddr, regType, data);
        // 加wapper 更新写缓存表
        ret = true;
    }
    return ret;
}
void BasePLCDevice::updateReadInfo()
{
    thUpdate_ = std::thread([this] {
        std::vector<uint16_t> readCache(readCacheSize_);
        while (updateHolder_.load(std::memory_order_acquire))
        {
            if (client_ != nullptr && client_->getConnection())
            {
                if (client_->readCache(readBeginAddress_, readCacheSize_, readCache))
                {
                    parsingReadInfo(readCache.data(), readCacheSize_);
                }
                int8_t readCount = 100;
                while (readCount)
                {
                    auto realInfo = client_->readRealtimeInfo();
                    if (realInfo.cache.size() > 0)
                    {
                        parsingRealtimeInfo(realInfo.cache.data(), realInfo.cache.size());
                    }
                    readCount--;
                    std::this_thread::sleep_for(std::chrono::milliseconds(1));
                }
            }
        }
    });
}
bool BasePLCDevice::getConnect()
{
    return client_->getConnection();
}
