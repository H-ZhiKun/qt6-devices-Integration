#include "ModbusClient.h"
#include <QDebug>
#include <bitset>

ModbusClient::ModbusClient(const ModbusInitArguments &args) : args_(args)
{
    rCaches_.resize(args_.rSize);
    uint16_t readAddrTemp = args_.rStartAddr;
    int32_t readSizeTemp = args_.rSize;
    while (readSizeTemp > 0)
    {
        RegisterReadData reader;
        reader.rStartAddr = readAddrTemp;
        uint16_t curSize = ((readSizeTemp - 100) < 0) ? readSizeTemp : 100;
        reader.rSize = curSize;
        vReadData.emplace_back(reader);
        readAddrTemp += 100;
        readSizeTemp -= 100;
    }
    // 启动连接线程
    keepConnection();
}

ModbusClient::~ModbusClient()
{
    // 设置线程退出条件为false
    bConnected_ = false;
    bThreadHolder_ = false;

    // 唤醒连接线程并等待所有任务线程结束
    cvConnector_.notify_all();

    for (auto &it : tasks_)
    {
        it.join();
    }
    if (mbsContext_ != nullptr)
    {
        modbus_close(mbsContext_);
        modbus_free(mbsContext_);
        mbsContext_ = nullptr;
    }
}

void ModbusClient::work()
{
    // 根据启动参数列表创建任务线程
    tasks_.emplace_back(std::thread([this]() {
        while (bThreadHolder_)
        {
            if (bConnected_)
            {
                // 缓存写
                while (!qWriteData_.empty())
                {
                    RegisterWriteData wData;
                    qWriteData_.dequeue(wData);
                    writeRegisters(wData.wStartAddr, wData.wSize, wData.wData);
                }
                // 读缓存
                for (auto const &readOne : vReadData)
                {
                    readRegisters(readOne.rStartAddr, readOne.rSize);
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
            }
            else
            {
                std::this_thread::sleep_for(std::chrono::seconds(2));
            }
        }
    }));
}

bool ModbusClient::readDatas(uint16_t address, uint16_t count, std::vector<uint16_t> &outData)
{
    // 检查是否越界
    uint16_t addr = address - args_.rStartAddr;
    if (addr + count > rCaches_.size())
    {
        // 如果请求的范围超过了缓存的容量，返回一个空的结果向量
        return false;
    }
    // 使用std::copy进行批量读取
    std::lock_guard lock(mtxRCache_); // 加缓存读锁
    std::copy(rCaches_.begin() + addr, rCaches_.begin() + addr + count, std::back_inserter(outData));

    return true;
}

bool ModbusClient::writeDatas(uint16_t address, WriteRegisterType type, const uint16_t *data)
{
    bool ret = true;
    uint16_t cacheIndex = address - args_.wStartAddr;
    if (cacheIndex >= wCaches_.size())
    {
        return false;
    }
    switch (type)
    {
    case WriteRegisterType::RegBool: {
        auto tempVal = std::bitset<16>(wCaches_[cacheIndex]);
        tempVal.set(data[0], data[1]);
        uint16_t uint16Val = tempVal.to_ulong();
        wCaches_[cacheIndex] = uint16Val;
        qWriteData_.enqueue(RegisterWriteData(address - 1, uint16Val));
        break;
    }
    case WriteRegisterType::RegInt: {
        wCaches_[cacheIndex] = data[0];
        qWriteData_.enqueue(RegisterWriteData(address - 1, data[0]));
        break;
    }
    case WriteRegisterType::RegReal: {
        wCaches_[cacheIndex] = data[0];
        wCaches_[cacheIndex + 1] = data[1];
        qWriteData_.enqueue(RegisterWriteData(address - 1, data));
        break;
    }

    default:
        break;
    }
    return ret;
}

bool ModbusClient::getConnection()
{
    return bConnected_;
}

bool ModbusClient::readRegisters(uint16_t address, uint16_t count)
{
    bool ret = true;
    static std::vector<uint16_t> readBuffer;
    readBuffer.resize(count);
    int result = modbus_read_registers(mbsContext_, address, count, readBuffer.data());
    if (result == -1)
    {
        LogError("Failed to read registers");
        bConnected_ = false;
        cvConnector_.notify_all();
        ret = false;
    }
    else
    {
        std::lock_guard lock(mtxRCache_);
        std::copy(readBuffer.begin(), readBuffer.end(), rCaches_.begin() + (address - args_.rStartAddr));
        qDebug() << fmt::format("updated by addr = {}, size = {}", address, result);
    }
    return ret;
}

bool ModbusClient::writeRegisters(uint16_t address, uint16_t size, const uint16_t *values)
{
    bool ret = true;
    if (modbus_write_registers(mbsContext_, address, size, values) == -1)
    {
        LogError("Failed to write registers");
        bConnected_ = false;
        cvConnector_.notify_all();
        ret = false;
    }
    return ret;
}

void ModbusClient::keepConnection()
{
    tasks_.emplace_back(std::thread([this]() {
        while (bThreadHolder_)
        {
            std::unique_lock<std::mutex> lock(mtxMbs_);

            // 等待连接断开或者退出条件
            cvConnector_.wait(lock, [this] { return bConnected_ == false; });

            // 检查线程退出条件
            if (bThreadHolder_ == false)
                return;

            // 关闭并释放之前的Modbus上下文
            if (mbsContext_ != nullptr)
            {
                modbus_close(mbsContext_);
                modbus_free(mbsContext_);
                mbsContext_ = nullptr;
            }

            // 创建新的Modbus上下文并连接
            mbsContext_ = modbus_new_tcp(args_.ip.c_str(), args_.port);
            modbus_set_slave(mbsContext_, 1);
            int nRet = modbus_connect(mbsContext_);

            if (-1 == nRet)
            {
                // 连接失败，等待1秒后重试
                std::this_thread::sleep_for(std::chrono::seconds(1));
                continue;
            }

            // 设置响应超时时间
            struct timeval tv;
            tv.tv_sec = 0;
            tv.tv_usec = 2000000;
            modbus_set_response_timeout(mbsContext_, tv.tv_sec, tv.tv_usec);
            bConnected_ = true;
        }
    }));
}
