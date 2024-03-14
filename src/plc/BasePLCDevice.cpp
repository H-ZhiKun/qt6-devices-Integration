#include "BasePLCDevice.h"
#include "AlertWapper.h"
#include "Logger.h"
#include "ModbusClient.h"
#include "Snap7Client.h"
#include <QFile>
#include <cstdint>
#include <mutex>
#include <qDebug.h>
#include <string>
#include <vector>

BasePLCDevice::BasePLCDevice(QObject *parent) : QObject(parent)
{
}

BasePLCDevice::~BasePLCDevice()
{
    updateHolder_.store(false, std::memory_order_release);
    if (thUpdate_.joinable())
    {
        thUpdate_.join();
    }
}
bool BasePLCDevice::init(const YAML::Node &config, const QString &path)
{
    auto ip = config["plc"]["host"].as<std::string>();
    protocol_ = config["plc"]["protocol"].as<std::string>();
    // 配置地址与报警文件
    if (!initAddress(path))
    {
        LogError("plc format address error.");
        return false;
    }
    if (!initAlert(path))
    {
        LogError("plc format alert error.");
        return false;
    }
    if (protocol_ == "snap7")
    {
        client_ = std::make_unique<Snap7Client>(ip);
    }
    else if (protocol_ == "modbus")
    {
        client_ = std::make_unique<ModbusClient>(ip, 502);
    }
    thUpdate_ = std::thread([this] {
        AlertWapper::modifyAllStatus();
        while (updateHolder_.load(std::memory_order_acquire))
        { // 初始化完成会退出，进入正常数据同步逻辑
            if (afterInit())
                break;
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
        if (updateHolder_.load(std::memory_order_acquire))
            updateRealTimeInfo();
    });
    return true;
}

uint32_t BasePLCDevice::getPlcCount(PLCCount)
{
    return 0;
}

bool BasePLCDevice::initAddress(const QString &path)
{
    QFile file(path + "/address.format");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        LogError("Failed to open address file.");
        return false;
    }
    // 读取文件内容
    QTextStream in(&file);
    while (!in.atEnd())
    {
        QString line = in.readLine();
        QStringList parts = line.split(",");
        if (parts.size() != 4)
        {
            qDebug() << "Invalid line format: " << line;
            return false;
        }

        QString key = parts[0];
        QString modbusInfo = parts[1];
        QString s7Info = parts[2];
        QString typeInfo = parts[3];
        std::vector<uint16_t> vAddress(4);
        if (protocol_ == "snap7")
        {
            auto addressBase = s7Info.split("_");
            vAddress[0] = addressBase[0].toUInt(); // 块编号
            vAddress[1] = addressBase[1].toUInt(); // 地址编号
            if (addressBase.size() > 2)
                vAddress[2] = addressBase[2].toUInt(); // 位编号
        }
        else if (protocol_ == "modbus")
        {
            auto addressBase = modbusInfo.split("_");
            vAddress[0] = 0;                       // 块编号
            vAddress[1] = addressBase[0].toUInt(); // 地址编号
            if (addressBase.size() > 1)
                vAddress[2] = addressBase[1].toUInt(); // 位编号
        }
        else
        {
            return false;
        }
        // 数据类型
        if (typeInfo == "bool")
            vAddress[3] = 1;
        else if (typeInfo == "int")
            vAddress[3] = 2;
        else if (typeInfo == "real")
            vAddress[3] = 3;
        mapAddressInfo_[key.toStdString()] = vAddress;
    }
    file.close();
    return true;
}
bool BasePLCDevice::initAlert(const QString &path)
{
    QFile file(path + "/alert.format");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        LogError("Failed to open alert file.");
        return false;
    }
    // 读取文件内容
    QTextStream in(&file);
    while (!in.atEnd())
    {
        QString line = in.readLine();
        QStringList parts = line.split(",");
        if (parts.size() != 2)
        {
            qDebug() << "Invalid line format: " << line;
            return false;
        }
        mapAlertStore_[parts[0].toStdString()] = parts[1].toStdString();
    }
    file.close();
    return true;
}

void BasePLCDevice::updateCaches(uint16_t address, const std::vector<uint8_t> &candidates)
{
    std::lock_guard lock(mtxCaches_);
    std::copy(candidates.begin(), candidates.end(), caches_.begin() + (address - cacheAddress_));
}

bool BasePLCDevice::isConnected()
{
    if (client_ == nullptr)
        return false;
    return client_->isConnected();
}

uint8_t BasePLCDevice::getDeviceStatus(PLCStateType states)
{
    switch (states)
    {
    case PLCStateType::PLCStates: {
        return plcStates_;
    }
    case PLCStateType::PLCSteps: {
        return plcSteps_;
    }
    default:
        break;
    }
    return plcStates_;
}
uint8_t BasePLCDevice::readBitValue(const uint16_t addr, const uint16_t bitPos)
{
    return (caches_[addr - cacheAddress_] >> bitPos) & 0x01;
}
std::string BasePLCDevice::readDevice(const std::string &readParams)
{
    std::string ret;
    if (!isConnected())
        return ret;
    auto finder = mapAddressInfo_.find(readParams);
    if (finder == mapAddressInfo_.end())
        return ret;
    const auto &dbNumber = finder->second[0];
    const auto &addr = finder->second[1];
    const auto &bit = finder->second[2];
    const auto &dataType = finder->second[3];
    switch (dataType)
    {
    case 1: { // 写入bit位是重载函数，其他类型是模版特化
        ret = std::to_string(readBitValue(addr, bit));
        break;
    }
    case 2: { // int
        ret = std::to_string(readSingle<uint16_t>(addr));
        break;
    }
    case 3: { // real
        ret = std::to_string(readSingle<float>(addr));
        break;
    }
    default:
        break;
    }
    return ret;
}
bool BasePLCDevice::writeBitValue(const uint16_t dbNumber, const uint16_t addr, const uint16_t bitPos, uint8_t value)
{
    if (client_ == nullptr)
        return false;
    return client_->writeBit(dbNumber, addr, bitPos, value);
}

bool BasePLCDevice::writeDevice(const std::string &key, const std::string &value)
{
    bool ret = false;
    if (!isConnected())
        return ret;
    auto finder = mapAddressInfo_.find(key);
    if (finder == mapAddressInfo_.end())
        return ret;
    const auto &dbNumber = finder->second[0];
    const auto &addr = finder->second[1];
    const auto &bit = finder->second[2];
    const auto &dataType = finder->second[3];

    switch (dataType)
    {
    case 1: { // 同read版本
        uint8_t data = value.empty() ? 0 : std::stoi(value);
        ret = writeBitValue(dbNumber, addr, bit, data);
        break;
    }
    case 2: { // int
        uint16_t data = value.empty() ? 0 : std::stoi(value);
        ret = writeSingle<uint16_t>(dbNumber, addr, data);
        break;
    }
    case 3: { // real
        float data = value.empty() ? 0 : std::stof(value);
        ret = writeSingle<float>(dbNumber, addr, data);
        break;
    }
    default:
        break;
    }
    return ret;
}

void BasePLCDevice::zeroClear()
{
    prudctOutNumber_ = 0;
    productCameraNumber_ = 0;
    prudctInNumber_ = 0;
}
