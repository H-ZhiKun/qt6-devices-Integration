#include "PLCDevice.h"
#include "AlertWapper.h"
#include <QDebug>
#include <bitset>
PLCDevice::PLCDevice()
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

void PLCDevice::init()
{
    ModbusInitArguments args;
    args.ip = "127.0.0.1";
    args.port = 502;
    client_ = new ModbusClient(std::move(args));
    client_->addWriteCache(writeBeginAddress_, writeCacheSize_);
    client_->addReadCache(readBeginAddress_, readCacheSize_, 500);
    client_->addFIFOCache(FIFOBeginAddress_, FIFOCacheSize_, 50);
    client_->work();
    updateData();
}

bool PLCDevice::writeDataToDevice(std::string addr, std::string type, std::string value)
{
    bool ret = false;
    if (!addr.empty() && !type.empty() && !value.empty())
    {
        uint16_t data[2] = {};
        uint16_t plcAddr = 0;
        WriteRegisterType regType;
        if (type == "bool")
        {
            if (addr.find('_') == std::string::npos)
            {
                return ret;
            }
            std::string subAddr = addr.substr(0, addr.find_first_of('_'));
            std::string bitAddr = addr.substr(addr.find_first_of('_') + 1);
            plcAddr = Utils::anyFromString<uint16_t>(subAddr);
            data[0] = Utils::anyFromString<uint16_t>(bitAddr);
            data[1] = Utils::anyFromString<uint16_t>(value);
            regType = WriteRegisterType::RegBool;
        }
        else if (type == "int")
        {
            plcAddr = Utils::anyFromString<uint16_t>(addr);
            data[0] = Utils::anyFromString<uint16_t>(value);
            regType = WriteRegisterType::RegInt;
        }
        else if (type == "real")
        {
            plcAddr = Utils::anyFromString<uint16_t>(addr);
            uint32_t uint32Val = Utils::anyFromString<float>(value) * 100;
            data[0] = (uint16_t)uint32Val;
            data[1] = (uint16_t)(uint32Val >> 16);
            regType = WriteRegisterType::RegReal;
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

void PLCDevice::updateData()
{
    thUpdate_ = std::thread([this] {
        std::vector<uint16_t> readCache;
        readCache.resize(readCacheSize_);
        AlertWapper::modifyAllStatus();
        while (updateHolder_.load(std::memory_order_acquire))
        {
            if (client_ != nullptr && client_->getConnection())
            {
                readCache.clear();
                if (client_->readCache(readBeginAddress_, 22, readCache))
                {
                    alertParsing(readCache.data(), 22);
                }
                int8_t fifoCount = 10;
                while (fifoCount)
                {
                    auto FIFO = client_->readFIFO();
                    if (FIFO.cache.size() > 0)
                    {
                        FIFOParsing(FIFO.cache.data(), FIFO.cache.size());
                    }
                    fifoCount--;
                    std::this_thread::sleep_for(std::chrono::milliseconds(50));
                }
            }
        }
    });
}

void PLCDevice::alertParsing(const uint16_t *alertGroup, uint16_t size)
{
    std::map<std::string, std::string> mapRealAlertInfo;
    // 12289 ~ 12310
    const uint16_t baseAddress = readBeginAddress_ + 1; // 预先计算起始地址

    for (uint16_t i = 0; i < size; i++)
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
                    std::cout << "PLC Address = " << key << std::endl;

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

void PLCDevice::FIFOParsing(const uint16_t *FIFOGroup, uint16_t size)
{
}
