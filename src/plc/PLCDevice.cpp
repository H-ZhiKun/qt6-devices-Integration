#include "PLCDevice.h"
#include "AlertWapper.h"
#include <QDebug>
#include <bitset>
PLCDevice::PLCDevice(QObject *parent) : QObject(parent)
{
}

PLCDevice::~PLCDevice()
{
    updateHolder_.store(false, std::memory_order_release);
    thUpdate_.join();
    if (client_)
    {
        delete client_;
        client_ = nullptr;
    }
    // 加wapper 清除写缓存表
}

void PLCDevice::init(const YAML::Node &config)
{
    std::string strType = config["plc"]["type"].as<std::string>();
    if (strType == "circle")
    {
        devType = DeviceType::CircleDevice;
    }
    else if (strType == "line")
    {
        devType = DeviceType::LineDevice;
    }
    else if (strType == "cap")
    {
        devType = DeviceType::CapDevice;
    }
    ModbusInitArguments args;
    for (const auto &item : config["plc"]["details"])
    {
        if (item["name"].as<std::string>() == strType)
        {
            args.ip = item["host"].as<std::string>();
            args.port = item["port"].as<uint16_t>();
            std::string normalAddress = item["normal_address"].as<std::string>();
            std::string writeAddress = item["write_address"].as<std::string>();
            std::string realtimeAddress = item["realtime_address"].as<std::string>();
            readBeginAddress_ = Utils::anyFromString<uint16_t>(normalAddress.substr(1)) - 1;
            readCacheSize_ = item["normal_size"].as<uint16_t>();
            writeBeginAddress_ = Utils::anyFromString<uint16_t>(writeAddress.substr(1)) - 1;
            writeCacheSize_ = item["write_size"].as<uint16_t>();
            realtimeBeginAddress_ = Utils::anyFromString<uint16_t>(realtimeAddress.substr(1)) - 1;
            realtimeCacheSize_ = item["realtime_size"].as<uint16_t>();
            break;
        }
    }
    client_ = new ModbusClient(std::move(args));
    client_->addWriteCache(writeBeginAddress_, writeCacheSize_);
    client_->addNormalCache(readBeginAddress_, readCacheSize_);
    client_->addRealtimeCache(realtimeBeginAddress_, realtimeCacheSize_);
    client_->work();
    updateReadInfo();
}

std::string PLCDevice::readDevice(const std::string &type, const std::string &addr, const std::string &bit)
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
                ret = std::to_string(data[0]);
            }
        }
    }
    return ret;
}

bool PLCDevice::writeDevice(const std::string &type, const std::string &addr, const std::string &bit,
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
            data[0] = Utils::anyFromString<uint16_t>(value);

            regType = WriteRegisterType::RegDInt;
        }

        client_->writeCache(plcAddr, regType, data);
        // 加wapper 更新写缓存表
        ret = true;
    }
    return ret;
}

const FIFOInfo &PLCDevice::getFIFOInfo()
{
    return fifoInfo_;
}

void PLCDevice::updateReadInfo()
{
    thUpdate_ = std::thread([this] {
        std::vector<uint16_t> readCache(readCacheSize_);
        AlertWapper::modifyAllStatus();
        while (updateHolder_.load(std::memory_order_acquire))
        {
            if (client_ != nullptr && client_->getConnection())
            {
                if (client_->readCache(readBeginAddress_, readCacheSize_, readCache))
                {
                    alertParsing(readCache.data(), readCacheSize_);
                }
                int8_t readCount = 500;
                while (readCount)
                {
                    auto realInfo = client_->readRealtimeInfo();
                    if (realInfo.cache.size() > 0)
                    {
                        realParsing(realInfo.cache.data(), realInfo.cache.size());
                    }
                    readCount--;
                    std::this_thread::sleep_for(std::chrono::milliseconds(1));
                }
            }
        }
    });
}

void PLCDevice::alertParsing(const uint16_t *alertGroup, uint16_t size)
{
    switch (devType)
    {
    case DeviceType::CircleDevice:
        circleAlertParing(alertGroup, size);
        break;
    case DeviceType::LineDevice:
        break;
    case DeviceType::CapDevice:
        break;
    default:
        break;
    }
}

void PLCDevice::circleAlertParing(const uint16_t *alertGroup, uint16_t size)
{
    std::map<std::string, std::string> mapRealAlertInfo;
    // 12289 ~ 12310
    const uint16_t baseAddress = readBeginAddress_ + 1; // 预先计算起始地址

    for (uint16_t i = 0; i < 22; i++)
    {
        if (alertGroup[i] > 0)
        {
            std::bitset<16> temp(alertGroup[i]);

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

void PLCDevice::realParsing(const uint16_t *realGroup, uint16_t size)
{
    switch (devType)
    {
    case DeviceType::CircleDevice:
        FIFOParsing(realGroup, size);
        break;
    case DeviceType::LineDevice:
        lineParsing(realGroup, size);
        break;
    case DeviceType::CapDevice:
        capParsing(realGroup, size);
        break;
    default:
        break;
    }
}

void PLCDevice::FIFOParsing(const uint16_t *FIFOGroup, uint16_t size)
{
    fifoInfo_.numPosition = FIFOGroup[2];
    fifoInfo_.numVerifyPos = FIFOGroup[3];
    fifoInfo_.numCoding = FIFOGroup[4];
    fifoInfo_.numVerifyCoding = FIFOGroup[5];
    fifoInfo_.signalMove = FIFOGroup[12];
    if (FIFOGroup[1] != fifoInfo_.numQRCode)
    {
        fifoInfo_.numQRCode = FIFOGroup[1];
        emit bottleMove(fifoInfo_.numQRCode);
    }
}

void PLCDevice::lineParsing(const uint16_t *lineGroup, uint16_t size)
{
    std::bitset<16> bit = std::bitset<16>(lineGroup[0]);
    if (bit[8] != lineInfo_.sigCoding)
    {
        emit lineCoding();
    }
    if (bit[9] != lineInfo_.sigCognex)
    {
        emit lineCognex();
    }
    lineInfo_.sigCoding = bit[8];
    lineInfo_.sigCognex = bit[9];
}

void PLCDevice::capParsing(const uint16_t *capGroup, uint16_t size)
{
    if (capGroup[0] != lineInfo_.sigCognex)
    {
        emit capCognex();
    }
    capInfo_.sigCognex = capGroup[0];
}

bool PLCDevice::getConnect()
{
    return client_->getConnection();
}
