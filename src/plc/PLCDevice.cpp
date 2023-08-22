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
    args.rStartAddr = 12288;
    args.rSize = 353;
    args.rClock = 500;
    client_ = new ModbusClient(std::move(args));
    client_->work();
    addrStartPLC_ = 12288;
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
        client_->writeDatas(plcAddr, regType, data);
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
                if (client_->readDatas(addrStartPLC_, 400, readCache))
                {
                    alertParsing(readCache.data(), 0);
                    motorBoolParsing(readCache.data(), 26);
                    motorRealParsing(readCache.data(), 33);
                    sensorParsing(readCache.data(), 269);
                    volveParsing(readCache.data(), 277);
                    produceRealParsing(readCache.data(), 281);
                    produceIntParsing(readCache.data(), 323);
                    produceWordParsing(readCache.data(), 325);
                    fifoBoolParsing(readCache.data(), 353);
                    fifoIntParsing(readCache.data(), 354);
                }
            }
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    });
}

void PLCDevice::alertParsing(const uint16_t *alertGoup, uint16_t start)
{
    static std::map<std::string, std::string> mapRealAlertInfo; // 需要默认排序，所以采用红黑树结构
    mapRealAlertInfo.clear();
    // 12289 ~ 12314
    // 0 ~ 25
    for (uint16_t i = start; i < 25; i++) // 读报警信息
    {
        if (alertGoup[i] > 0)
        {
            std::bitset<16> temp(alertGoup[i]);
            for (uint8_t j = 0; j < 16; j++)
            {
                if (temp.test(j))
                {
                    // 首编号4 是PLC保持寄存器类型号
                    std::string key = fmt::format("4{}_{}", addrStartPLC_ + i, j);
                    std::cout << "plc addr: " << key << std::endl;
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

void PLCDevice::motorBoolParsing(const uint16_t *alertGoup, uint16_t start)
{
    // 读bool数据 电机状态、电机手动模式、一键自动、所有轴回0
    // 12315 ~ 12321
    // 26 ~ 32
    for (uint16_t i = start; i < start + 7; i++)
    {
        if (alertGoup[i] > 0)
        {
            std::bitset<16> temp(alertGoup[i]);
            for (uint8_t j = 0; j < 16; j++)
            {
                if (temp.test(j))
                {
                    std::string key = fmt::format("4{}_{}", plcAddr + i, j);
                    LogInfo("plc addr: {}", key);
                }
            }
        }
    }
}

void PLCDevice::motorRealParsing(const uint16_t *alertGoup, uint16_t start, uint16_t plcAddr)
{
    // 读real数据, 电机实际位置
    // 412322 ~ 412509
    // 33 ~ 220
    for (uint16_t i = start; i < start + 187; i += 2)
    {
        std::vector<float> realData;
        if (alertGoup[i] > 0 || alertGoup[i + 1] > 0)
        {
            if (i + 1 < start + 188) // Make sure there's enough data for both parts
            {
                // uint32_t combinedData = (static_cast<uint32_t>(alertGoup[i + 1]) << 16) | alertGoup[i];

                // Extract low and high parts of the combined data
                uint16_t lowPart = alertGoup[i] & 0xFFFF;
                uint16_t highPart = alertGoup[i + 1] & 0xFFFF;

                // Combine and adjust the data
                uint32_t adjustedData = static_cast<uint32_t>((highPart << 16) | lowPart);

                // Convert to float and apply the 100x scaling
                float floatValue = static_cast<float>(adjustedData);
                floatValue /= 100;
                std::string key = fmt::format("4{}_{}", plcAddr + i, floatValue);
                LogInfo("plc addr: {}", key);
                realData.push_back(floatValue);
            }
        }
        // 处理 realData
    }
}

void PLCDevice::sensorParsing(const uint16_t *alertGoup, uint16_t start, uint16_t plcAddr)
{
    // 读bool数据，传感器状态
    // 412558 ~ 412563.5
    // 269 ~ 274.5
    for (uint16_t i = start; i < start + 7; i += 1)
    {
        if (alertGoup[i] > 0)
        {
            std::bitset<16> temp(alertGoup[i]);
            for (uint8_t j = 0; j < 16; j++)
            {
                if (temp.test(j))
                {
                    std::string key = fmt::format("4{}_{}", plcAddr + i, j);
                    LogInfo("plc addr: {}", key);
                }
            }
        }
    }
}

void PLCDevice::volveParsing(const uint16_t *alertGoup, uint16_t start, uint16_t plcAddr)
{
    // 读bool数据，电磁阀状态
    // 412566 ~ 412568.25
    // 277 ~ 279.25
    for (uint16_t i = start; i < start + 4; i += 1)
    {
        if (alertGoup[i] > 0)
        {
            std::bitset<16> temp(alertGoup[i]);
            for (uint8_t j = 0; j < 16; j++)
            {
                if (temp.test(j))
                {
                    std::string key = fmt::format("4{}_{}", plcAddr + i, j);
                    LogInfo("plc addr: {}", key);
                }
            }
        }
    }
}

void PLCDevice::produceRealParsing(const uint16_t *alertGoup, uint16_t start, uint16_t plcAddr)
{
    // 读real数据，生产显示页面
    // 412570 ~ 412611
    // 281 ~ 322
    for (uint16_t i = start; i < start + 41; i += 2)
    {
        std::vector<float> realData;
        if (alertGoup[i] > 0 || alertGoup[i + 1] > 0)
        {
            if (i + 1 < 400) // Make sure there's enough data for both parts
            {
                uint32_t combinedData = (static_cast<uint32_t>(alertGoup[i + 1]) << 16) | alertGoup[i];

                // Extract low and high parts of the combined data
                uint16_t lowPart = combinedData & 0xFFFF;
                uint16_t highPart = (combinedData >> 16) & 0xFFFF;

                // Combine and adjust the data
                int32_t adjustedData = static_cast<int32_t>((highPart << 16) | lowPart);

                // Convert to float and apply the 100x scaling
                float floatValue = static_cast<float>(adjustedData) / 100.0f;
                std::string key = fmt::format("4{}_{}", plcAddr + i, floatValue);
                LogInfo("plc addr: {}", key);
                realData.push_back(floatValue);
            }
        }
        // 处理 realData
    }
}

void PLCDevice::produceIntParsing(const uint16_t *alertGoup, uint16_t start, uint16_t plcAddr)
{
    // 读int数据，生产显示画面
    // 412612 ~ 412613
    // 323 ~ 324
    for (uint16_t i = start; i < start + 2; i += 1)
    {
        if (alertGoup[i] > 0)
        {
            int16_t result = static_cast<int16_t>(alertGoup[i]);
            std::string key = fmt::format("4{}_{}", plcAddr + i, result);
            LogInfo("plc addr: {}", key);
        }
    }
}

void PLCDevice::produceWordParsing(const uint16_t *alertGoup, uint16_t start, uint16_t plcAddr)
{
    // 读word数据，年与日时分秒
    // 412614 ~ 412619
    // 325 ~ 330
    for (uint16_t i = start; i < start + 6; i += 1)
    {
        if (alertGoup[i] > 0)
        {
            uint16_t wordData = alertGoup[i];
            std::string key = fmt::format("4{}_{}", plcAddr + i, wordData);
            LogInfo("plc addr: {}", key);
        }
    }
}

void PLCDevice::fifoBoolParsing(const uint16_t *alertGoup, uint16_t start, uint16_t plcAddr)
{
    // 读bool数据，瓶位移动信号、喷码数据查找信号
    // 412642
    // 353
    for (uint16_t i = start; i < start + 1; i += 1)
    {
        if (alertGoup[i] > 0)
        {
            std::bitset<16> temp(alertGoup[i]);
            for (uint8_t j = 0; j < 16; j++)
            {
                if (temp.test(j))
                {
                    std::string key = fmt::format("4{}_{}", plcAddr + i, j);
                    LogInfo("plc addr: {}", key);
                }
            }
        }
    }
}

void PLCDevice::fifoIntParsing(const uint16_t *alertGoup, uint16_t start, uint16_t plcAddr)
{
    // 读int数据，fifo数据
    // 412643 ~ 412655
    // 354 ~ 366
    for (uint16_t i = start; i < start + 13; i += 1)
    {
        if (alertGoup[i] > 0)
        {
            // 瓶位移动信号
            if (i == 365)
            {
                // emit bottomMove(); // 瓶位移动信号
            }
            int16_t result = static_cast<int16_t>(alertGoup[i]);
            std::string key = fmt::format("4{}_{}", plcAddr + i, result);
            LogInfo("plc addr: {}", key);
        }
    }
}