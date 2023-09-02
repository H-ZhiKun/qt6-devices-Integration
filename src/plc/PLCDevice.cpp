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

void PLCDevice::init(const std::string &host, uint16_t port, uint16_t ioFreq, uint16_t FIFOFreq)
{
    ModbusInitArguments args;
    args.ip = host;
    args.port = port;
    client_ = new ModbusClient(std::move(args));
    client_->addWriteCache(writeBeginAddress_, writeCacheSize_);
    client_->addReadCache(readBeginAddress_, readCacheSize_, ioFreq);
    client_->addFIFOCache(FIFOBeginAddress_, FIFOCacheSize_, FIFOFreq);
    client_->work();
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
                int8_t fifoCount = 500;
                while (fifoCount)
                {
                    auto FIFO = client_->readFIFO();
                    if (FIFO.cache.size() > 0)
                    {
                        FIFOParsing(FIFO.cache.data(), FIFO.cache.size());
                    }
                    fifoCount--;
                    std::this_thread::sleep_for(std::chrono::milliseconds(1));
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
    // if (FIFOGroup[1] != fifoInfo_.numQRCode)
    // {
    //     fifoInfo_.numQRCode = FIFOGroup[1];
    // }
    // if (FIFOGroup[2] != fifoInfo_.numPosition)
    // {
    //     fifoInfo_.numPosition = FIFOGroup[2];
    // }
    // if (FIFOGroup[3] != fifoInfo_.numVerifyPos)
    // {
    //     fifoInfo_.numVerifyPos = FIFOGroup[3];
    // }
    // if (FIFOGroup[4] != fifoInfo_.numCoding)
    // {
    //     fifoInfo_.numCoding = FIFOGroup[4];
    //     LogInfo("bottom {}: code Logistics signal", FIFOGroup[4]);
    //     emit codeLogistics(FIFOGroup[4]);
    // }
    // if (FIFOGroup[5] != fifoInfo_.numVerifyCoding)
    // {
    //     fifoInfo_.numVerifyCoding = FIFOGroup[5];
    //     LogInfo("bottom {}: photo code check signal", FIFOGroup[5]);
    //     emit codeCheck(FIFOGroup[5]);
    // }
    fifoInfo_.numPosition = FIFOGroup[2];
    fifoInfo_.numVerifyPos = FIFOGroup[3];
    fifoInfo_.numCoding = FIFOGroup[4];
    fifoInfo_.numVerifyCoding = FIFOGroup[5];
    fifoInfo_.signalMove = FIFOGroup[12];
    if (FIFOGroup[1] != fifoInfo_.numQRCode)
    {
        fifoInfo_.numQRCode = FIFOGroup[1];
        fifoInfo_.signalSearchCoding != FIFOGroup[13];
        emit bottomMove(fifoInfo_.numQRCode);
    }
}

bool PLCDevice::getConnect()
{
    return client_->getConnection();
}
