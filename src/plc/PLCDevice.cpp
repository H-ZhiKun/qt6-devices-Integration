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
                if (client_->readDatas(0, 400, readCache))
                {
                    // local index: 0~21 => 对应plc地址: 12289~12310
                    alertParsing(readCache.data(), 22, 12289);
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
