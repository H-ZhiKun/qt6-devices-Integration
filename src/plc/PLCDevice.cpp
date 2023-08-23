#include "PLCDevice.h"
#include "AlertWapper.h"
#include <QDebug>
#include <bitset>
PLCDevice::PLCDevice()
{
}

PLCDevice::~PLCDevice()
{
    if (client_)
    {
        delete client_;
        client_ = nullptr;
    }
    updateHolder_ = false;
    thUpdate_.join();
    // 加wapper 清除写缓存表
}

void PLCDevice::init()
{
    ModbusInitArguments args;
    args.ip = "127.0.0.1";
    args.port = 502;
    client_ = new ModbusClient(std::move(args));
    client_->addWriteCache(12688, 316);
    client_->addReadCache(12288, 331, 500);
    client_->addFIFOCache(12641, 14, 50);
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

void PLCDevice::updateData()
{
    thUpdate_ = std::thread([this] {
        std::vector<uint16_t> readCache;
        readCache.resize(400);
        AlertWapper::modifyAllStatus();
        while (updateHolder_)
        {
            if (client_->getConnection())
            {
                readCache.clear();
                if (client_->readCache(alertBeginAddress, 22, readCache))
                {
                    alertParsing(readCache.data(), 22);
                }
            }
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    });
}

void PLCDevice::alertParsing(const uint16_t *alertGoup, uint16_t size)
{
    static std::map<std::string, std::string> mapRealAlertInfo; // 需要默认排序，所以采用红黑树结构
    mapRealAlertInfo.clear();
    // 12289 ~ 12310
    for (uint16_t i = 0; i < size; i++) // 读报警信息
    {
        if (alertGoup[i] > 0)
        {
            std::bitset<16> temp(alertGoup[i]);
            for (uint8_t j = 0; j < 16; j++)
            {
                if (temp.test(j))
                {
                    // 首编号4 是PLC保持寄存器类型号
                    std::string key = fmt::format("4{}_{}", alertBeginAddress + i + 1, j);
                    std::cout << "PLC Address = " << key << std::endl;
                    auto finder = regWapper_.mapAlertInfo.find(key);
                    if (finder != regWapper_.mapAlertInfo.end())
                    {
                        mapRealAlertInfo[key] = finder->second;
                        qDebug() << "alert: " << QString::fromUtf8(finder->second.c_str());
                    }
                }
            }
        }
    }
    AlertWapper::updateRealtimeAlert(mapRealAlertInfo);
}