#include "ModbusClient.h"
#include <Logger.h>
#include <QDebug>

ModbusClient::ModbusClient(const std::string &ip, const uint16_t port) : ip_(ip), port_(port)
{
}

ModbusClient::~ModbusClient()
{
    // 设置线程退出条件为false
    bConnected_ = false;
    bExit_ = true;
    // 唤醒连接线程并等待所有任务线程结束
    cvConnector_.notify_one();

    if (mbsContext_ != nullptr)
    {
        modbus_close(mbsContext_);
        modbus_free(mbsContext_);
        mbsContext_ = nullptr;
    }
}

bool ModbusClient::isConnected()
{
    return bConnected_;
}

bool ModbusClient::readBit(const uint16_t dbNumber, const uint16_t address, const uint16_t bitPos, uint8_t &value)
{
    std::vector<uint16_t> words(1);
    if (!readWords(dbNumber, address, words))
        return false;
    // 提取位值
    value = (words[0] >> bitPos) & 0x0001;
    return true;
}

bool ModbusClient::readBytes(const uint16_t dbNumber, const uint16_t address, std::vector<uint8_t> &values)
{
    return true;
}

bool ModbusClient::readWords(const uint16_t dbNumber, const uint16_t address, std::vector<uint16_t> &values)
{
    if (!isConnected())
        return false;
    uint16_t remainingCount = values.size();
    // modbus tcp协议 寄存器地址 从0开始计数，而PLC地址从1开始，所以在这里统一减1
    uint16_t currentAddress = address - 1;
    // Modbus 协议规定一次读取请求的最大寄存器数量为 125 个（0x7D）。这是由于 Modbus 协议采用 16
    // 位的寄存器地址，因此最大数量为 2^15 - 1，但由于功能码和字节计数等信息的存在，实际可用数量略小于这个值
    // 在这里设置一次性读取最大个数为64，读取个数会影响单次读取时间
    uint16_t batchCount = std::min(remainingCount, static_cast<uint16_t>(64));
    std::lock_guard lock(mtxMbs_);
    while (remainingCount > 0)
    {
        int result = modbus_read_registers(mbsContext_, currentAddress, batchCount,
                                           values.data() + (values.size() - remainingCount));

        if (result == -1)
        {
            LogError("Failed to read registers={}, size={}", currentAddress, batchCount);
            bConnected_ = false;
            cvConnector_.notify_one();
            return false;
        }

        remainingCount -= batchCount;
        currentAddress += batchCount;
        batchCount = std::min(remainingCount, static_cast<uint16_t>(64));
    }
    return true;
}

bool ModbusClient::readDInts(const uint16_t dbNumber, const uint16_t address, std::vector<uint32_t> &values)
{
    std::vector<uint16_t> words(values.size() * 2);
    if (!readWords(dbNumber, address, words))
        return false;

    // 遍历values，每两个元素合成一个Dint
    for (size_t i = 0; i < values.size(); i++)
    {
        uint16_t low = words[2 * i];
        uint16_t high = words[2 * i + 1];
        uint32_t combinedValue = (static_cast<uint32_t>(high) << 16) | static_cast<uint32_t>(low);
        values[i] = combinedValue;
    }
    return true;
}

bool ModbusClient::readFloats(const uint16_t dbNumber, const uint16_t address, std::vector<float> &values)
{
    std::vector<uint16_t> words(values.size() * 2);
    if (!readWords(dbNumber, address, words))
        return false;

    // 遍历values，每两个元素合成一个float
    for (size_t i = 0; i < values.size(); i++)
    {
        uint16_t low = words[2 * i];
        uint16_t high = words[2 * i + 1];
        uint32_t combinedValue = (static_cast<uint32_t>(high) << 16) | static_cast<uint32_t>(low);
        values[i] = combinedValue / 100.0f;
    }
    return true;
}
bool ModbusClient::writeBit(const uint16_t dbNumber, const uint16_t address, const uint16_t bitPos, const uint8_t value)
{
    std::vector<uint16_t> registers(1);
    if (!readWords(dbNumber, address, registers))
        return false;
    // 设置指定位的值
    if (value)
        registers[0] |= (1 << bitPos); // 将指定位设置为 1
    else
        registers[0] &= ~(1 << bitPos); // 将指定位设置为 0

    // 写入修改后的寄存器值
    return writeWords(dbNumber, address, registers);
}

bool ModbusClient::writeBytes(const uint16_t dbNumber, const uint16_t address, const std::vector<uint8_t> &values)
{
    return true;
}

bool ModbusClient::writeWords(const uint16_t dbNumber, const uint16_t address, const std::vector<uint16_t> &values)
{
    if (!isConnected())
        return false;
    uint16_t currentAddress = address - 1;
    std::lock_guard lock(mtxMbs_);
    int result = modbus_write_registers(mbsContext_, currentAddress, values.size(), values.data());
    if (result == -1)
    {
        LogError("Failed to write registers");
        bConnected_ = false;
        cvConnector_.notify_one();
        return false;
    }
    return true;
}

bool ModbusClient::writeDInts(const uint16_t dbNumber, const uint16_t address, const std::vector<uint32_t> &values)
{
    std::vector<uint16_t> words(values.size() * 2);
    for (size_t i = 0; i < values.size(); i++)
    {
        uint16_t low = static_cast<uint16_t>(values[i] & 0xFFFF);
        uint16_t high = static_cast<uint16_t>((values[i] >> 16) & 0xFFFF);
        words[2 * i] = low;
        words[2 * i + 1] = high;
    }
    return writeWords(dbNumber, address, words);
}

bool ModbusClient::writeFloats(const uint16_t dbNumber, const uint16_t address, const std::vector<float> &values)
{
    std::vector<uint16_t> words(values.size() * 2);
    for (size_t i = 0; i < values.size(); i++)
    {
        uint32_t u32Value = static_cast<uint32_t>(values[i] * 100.0f);
        uint16_t low = static_cast<uint16_t>(u32Value & 0xFFFF);
        uint16_t high = static_cast<uint16_t>((u32Value >> 16) & 0xFFFF);
        words[2 * i] = low;
        words[2 * i + 1] = high;
    }
    return writeWords(dbNumber, address, words);
}
void ModbusClient::keepConnection()
{
    std::unique_lock lock(mtxMbs_);

    // 等待连接断开或者退出条件
    cvConnector_.wait(lock, [this] { return !bConnected_; });
    if (bExit_)
        return;
    // 关闭并释放之前的Modbus上下文
    if (mbsContext_ != nullptr)
    {
        modbus_close(mbsContext_);
        modbus_free(mbsContext_);
        mbsContext_ = nullptr;
    }

    // 创建新的Modbus上下文并连接
    mbsContext_ = modbus_new_tcp(ip_.c_str(), port_);
    modbus_set_slave(mbsContext_, 1);
    int nRet = modbus_connect(mbsContext_);

    if (-1 == nRet)
    {
        LogError("modbus connect failed.");
        return;
    }
    LogInfo("modbus connect success");
    // 设置响应超时时间
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 500000;
    modbus_set_response_timeout(mbsContext_, tv.tv_sec, tv.tv_usec);
    bConnected_ = true;
}
