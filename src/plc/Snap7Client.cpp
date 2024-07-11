#include "Snap7Client.h"
#include "snap7.h"
#include <Logger.h>
#include <QDebug>
#include <cstdint>
#include <vector>

Snap7Client::Snap7Client(const std::string &ip) : ip_(ip)
{
}

Snap7Client::~Snap7Client()
{
    // 设置线程退出条件为false
    bConnected_ = false;
    bExit_ = true;
    // 唤醒连接线程并等待所有任务线程结束
    cvConnector_.notify_one();
}

bool Snap7Client::isConnected()
{
    return bConnected_;
}
bool Snap7Client::readBit(const uint16_t dbNumber, const uint16_t address, const uint16_t bitPos, uint8_t &value)
{
    std::vector<uint8_t> bytes(1);
    if (!readBytes(dbNumber, address, bytes))
        return false;
    // 提取位值
    value = (bytes[0] >> bitPos) & 0x01;
    return true;
}

bool Snap7Client::readBytes(const uint16_t dbNumber, const uint16_t address, std::vector<uint8_t> &values)
{
    if (!isConnected())
        return false;
    std::lock_guard lock(mtxContext_);
    auto errorCode = context_.DBRead(dbNumber, address, values.size(), values.data());
    if (errorCode != 0)
    {
        LogError("Failed to read s7,addr = {}_{},size ={},  code = {}, str = {}", dbNumber, address, values.size(),
                 errorCode, CliErrorText(errorCode));
        if (!context_.Connected())
        {
            bConnected_ = false;
            cvConnector_.notify_one();
        }
        return false;
    }
    return true;
}
bool Snap7Client::readWords(const uint16_t dbNumber, const uint16_t address, std::vector<uint16_t> &values)
{
    std::vector<uint8_t> bytes(values.size() * 2);
    if (!readBytes(dbNumber, address, bytes))
        return false;
    // 将读取到的字节按照 每两个字节高低翻转的顺序组合成 uint16_t 值，并放入 values 中
    for (size_t i = 0; i < values.size(); ++i)
    {
        uint16_t highByte = bytes[i * 2];
        uint16_t lowByte = bytes[i * 2 + 1];
        values[i] = (highByte << 8) | lowByte;
    }
    return true;
}

bool Snap7Client::readDInts(const uint16_t dbNumber, const uint16_t address, std::vector<uint32_t> &values)
{
    std::vector<uint16_t> words(values.size() * 2);
    if (!readWords(dbNumber, address, words))
        return false;

    // 遍历values，每两个元素合成一个DInt
    for (size_t i = 0; i < values.size(); i++)
    {
        uint16_t low = words[2 * i];
        uint16_t high = words[2 * i + 1];
        uint32_t combinedValue = static_cast<uint32_t>(high) | (static_cast<uint32_t>(low) << 16);
        values[i] = combinedValue;
    }
    return true;
}

bool Snap7Client::readFloats(const uint16_t dbNumber, const uint16_t address, std::vector<float> &values)
{
    std::vector<uint16_t> words(values.size() * 2);
    if (!readWords(dbNumber, address, words))
        return false;

    // 遍历values，每两个元素合成一个float
    for (size_t i = 0; i < values.size(); i++)
    {
        uint16_t low = words[2 * i];
        uint16_t high = words[2 * i + 1];
        uint32_t combinedValue = static_cast<uint32_t>(high) | (static_cast<uint32_t>(low) << 16);
        values[i] = combinedValue / 100.0f;
    }
    return true;
}

bool Snap7Client::writeBit(const uint16_t dbNumber, const uint16_t address, const uint16_t bitPos, const uint8_t value)
{
    std::vector<uint8_t> bytes(1);
    if (!readBytes(dbNumber, address, bytes))
        return false;
    // 设置指定位的值
    if (value)
        bytes[0] |= (1 << bitPos); // 将指定位设置为 1
    else
        bytes[0] &= ~(1 << bitPos); // 将指定位设置为 0

    // 写入修改后的寄存器值
    return writeBytes(dbNumber, address, bytes);
}

bool Snap7Client::writeBytes(const uint16_t dbNumber, const uint16_t address, const std::vector<uint8_t> &values)
{
    if (!isConnected())
        return false;
    std::lock_guard lock(mtxContext_);
    // auto errorCode =
    //     context_.AsWriteArea(S7AreaDB, dbNumber, address, values.size(), S7WLByte, (uint8_t *)values.data());

    auto errorCode = context_.DBWrite(dbNumber, address, values.size(), (uint8_t *)values.data());

    if (errorCode != 0)
    {
        LogError("Failed to write s7, addr = {}_{},size ={}, code = {},str = {}", dbNumber, address, values.size(),
                 errorCode, CliErrorText(errorCode));
        if (!context_.Connected())
        {
            bConnected_ = false;
            cvConnector_.notify_one();
        }
        return false;
    }
    return true;
}
bool Snap7Client::writeWords(const uint16_t dbNumber, const uint16_t address, const std::vector<uint16_t> &values)
{
    std::vector<uint8_t> writeDatas(values.size() * 2);
    // 将 values 中的 uint16_t 值按照逆过程转换为字节序
    for (size_t i = 0; i < values.size(); ++i)
    {
        // 同样需要高低翻转写入
        writeDatas[i * 2] = static_cast<uint8_t>(values[i] >> 8); // 高8位放入低地址
        writeDatas[i * 2 + 1] = static_cast<uint8_t>(values[i]);  // 低8位放入高地址
    }
    return writeBytes(dbNumber, address, writeDatas);
}

bool Snap7Client::writeDInts(const uint16_t dbNumber, const uint16_t address, const std::vector<uint32_t> &values)
{
    std::vector<uint16_t> words(values.size() * 2);
    for (size_t i = 0; i < values.size(); i++)
    {
        uint16_t low = static_cast<uint16_t>(values[i] & 0xFFFF);
        uint16_t high = static_cast<uint16_t>((values[i] >> 16) & 0xFFFF);
        words[2 * i] = high;
        words[2 * i + 1] = low;
    }
    return writeWords(dbNumber, address, words);
}

bool Snap7Client::writeFloats(const uint16_t dbNumber, const uint16_t address, const std::vector<float> &values)
{
    std::vector<uint16_t> words(values.size() * 2);
    for (size_t i = 0; i < values.size(); i++)
    {
        uint32_t u32Value = static_cast<uint32_t>(values[i] * 100.0f);
        uint16_t low = static_cast<uint16_t>(u32Value & 0xFFFF);
        uint16_t high = static_cast<uint16_t>((u32Value >> 16) & 0xFFFF);
        words[2 * i] = high;
        words[2 * i + 1] = low;
    }
    return writeWords(dbNumber, address, words);
}

void Snap7Client::keepConnection()
{
    std::unique_lock lock(mtxContext_);
    // 等待连接断开或者退出条件
    cvConnector_.wait(lock, [this] { return !bConnected_; });
    if (bExit_)
        return;
    int errorCode = context_.ConnectTo(ip_.c_str(), rack_, slot_);
    if (errorCode == 0)
    {
        LogInfo("snap7 connect success");
        bConnected_ = true;
    }
    else
    {
        LogInfo("snap7 connect failed,code = {}", errorCode);
    }
}