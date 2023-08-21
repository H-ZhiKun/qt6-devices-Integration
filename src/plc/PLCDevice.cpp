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
}

void PLCDevice::init()
{
    ModbusInitArguments args;
    args.ip = "127.0.0.1";
    args.port = 502;
    args.rStartAddr = 12288;
    args.rSize = 400;
    args.rClock = 500;
    client_ = new ModbusClient(std::move(args));
    client_->work();
    updateData();
}

bool PLCDevice::writeDataToDevice(std::string addr, std::string type, std::string value)
{
    bool ret = false;
    if (!addr.empty() && !type.empty() && !value.empty())
    {
        if (type == "bool")
        {
            if (addr.find('_') == std::string::npos)
            {
                return ret;
            }
            std::string subAddr = addr.substr(0, addr.find_first_of('_'));
            std::string bitAddr = addr.substr(addr.find_first_of('_') + 1);
            uint16_t data[2] = {};
            uint16_t plcAddr = Utils::anyFromString<uint16_t>(subAddr);
            data[0] = Utils::anyFromString<uint16_t>(bitAddr);
            data[1] = Utils::anyFromString<uint16_t>(value);
            client_->writeDatas(plcAddr, WriteRegisterType::RegBool, data);
        }
        else if (type == "int")
        {
            uint16_t data[2] = {};
            uint16_t plcAddr = Utils::anyFromString<uint16_t>(addr);
            data[0] = Utils::anyFromString<uint16_t>(value);
            client_->writeDatas(plcAddr, WriteRegisterType::RegInt, data);
        }
        else if (type == "real")
        {
            uint16_t data[2] = {};
            uint16_t plcAddr = Utils::anyFromString<uint16_t>(addr);
            uint32_t uint32Val = Utils::anyFromString<float>(value) * 100;
            data[0] = (uint16_t)uint32Val;
            data[1] = (uint16_t)(uint32Val >> 16);

            client_->writeDatas(plcAddr, WriteRegisterType::RegReal, data);
        }
    }
    return ret;
}

void PLCDevice::updateData()
{
    thUpdate_ = std::thread([this] {
        uint16_t addrRead = 12288;
        std::vector<uint16_t> readCache;
        readCache.resize(400);
        AlertWapper::modifyAllStatus();
        while (updateHolder_)
        {
            if (client_->getConnection())
            {
                readCache.clear();
                if (client_->readDatas(addrRead, 400, readCache))
                {
                    // local index: 0~21 => 对应plc地址: 12289~12310
                    alertParsing(readCache.data(), 22, addrRead + 1);
                }
            }
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    });
}

void PLCDevice::alertParsing(const uint16_t *alertGoup, uint16_t size, uint16_t plcAddr)
{
    static std::map<std::string, std::string> mapRealAlertInfo; // 需要默认排序，所以采用红黑树结构
    mapRealAlertInfo.clear();
    for (uint16_t i = 0; i < size; i++)
    {
        if (alertGoup[i] > 0)
        {
            std::bitset<16> temp(alertGoup[i]);
            for (uint8_t j = 0; j < 16; j++)
            {
                if (temp.test(j))
                {
                    // 首编号4 是PLC保持寄存器类型号
                    std::string key = fmt::format("4{}_{}", plcAddr + i, j);
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
