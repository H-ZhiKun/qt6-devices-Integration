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
    if (deviceUpdate_)
    {
        delete deviceUpdate_;
        deviceUpdate_ = nullptr;
    }
    // 加wapper 清除写缓存表
}

void PLCDevice::init()
{
    ModbusInitArguments args;
    args.ip = "127.0.0.1";
    // args.ip = "192.168.1.10";
    args.port = 502;
    client_ = new ModbusClient(std::move(args));
    client_->addWriteCache(writeBeginAddress_, writeCacheSize_);
    client_->addReadCache(readBeginAddress_, readCacheSize_, 500);
    client_->addFIFOCache(FIFOBeginAddress_, FIFOCacheSize_, 50);
    client_->work();
    deviceUpdate_ = new DeviceUpdate();
    updateData();
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
        }
    }
    return ret;
}

bool PLCDevice::writeDataToDevice(const std::string &type, const std::string &addr, const std::string &bit,
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
        const uint16_t readSize = 22;
        std::vector<uint16_t> readCache(readSize);
        AlertWapper::modifyAllStatus();
        while (updateHolder_.load(std::memory_order_acquire))
        {
            if (client_ != nullptr && client_->getConnection())
            {
                if (client_->readCache(readBeginAddress_, readSize, readCache))
                {
                    alertParsing(readCache.data(), readSize);
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

void PLCDevice::FIFOParsing(const uint16_t *FIFOGroup, uint16_t size)
{
    if (FIFOGroup[1] != fifoInfo_.numQRCode.load(std::memory_order_relaxed))
    {
        deviceUpdate_->UpdateBottomMove(FIFOGroup[1]);
        fifoInfo_.numQRCode.store(FIFOGroup[1], std::memory_order_relaxed);
    }
    if (FIFOGroup[2] != fifoInfo_.numPosition.load(std::memory_order_relaxed))
    {
        emit deviceUpdate_->locatePhoto(0, FIFOGroup[2]);
        fifoInfo_.numPosition.store(FIFOGroup[2], std::memory_order_relaxed);
    }
    if (FIFOGroup[3] != fifoInfo_.numVerifyPos.load(std::memory_order_relaxed))
    {
        emit deviceUpdate_->locateCheckPhoto(2, FIFOGroup[3]);
        fifoInfo_.numVerifyPos.store(FIFOGroup[3], std::memory_order_relaxed);
    }
    if (FIFOGroup[4] != fifoInfo_.numCoding.load(std::memory_order_relaxed))
    {
        emit deviceUpdate_->codeLogistics(FIFOGroup[4]);
        fifoInfo_.numCoding.store(FIFOGroup[4], std::memory_order_relaxed);
    }
    if (FIFOGroup[5] != fifoInfo_.numVerifyCoding.load(std::memory_order_relaxed))
    {
        emit deviceUpdate_->codeCheck(1, FIFOGroup[5]);
        fifoInfo_.numVerifyCoding.store(FIFOGroup[5], std::memory_order_relaxed);
    }
    if (FIFOGroup[12] != fifoInfo_.signalMove.load(std::memory_order_relaxed))
    {
        emit deviceUpdate_->bottomMove(FIFOGroup[12]);
        fifoInfo_.signalMove.store(FIFOGroup[12], std::memory_order_relaxed);
    }
    if (FIFOGroup[13] != fifoInfo_.signalSearchCoding.load(std::memory_order_relaxed))
    {
        emit deviceUpdate_->codeSerch(FIFOGroup[13]);
        fifoInfo_.signalSearchCoding.store(FIFOGroup[13], std::memory_order_relaxed);
    }

    // std::cout << "FIFO updated: " << fifoInfo_.numQRCode << "," << fifoInfo_.signalSearchCoding
    //           << Utils::getCurrentTime(true) << std::endl;
}
