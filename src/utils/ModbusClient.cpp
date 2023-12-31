#include "ModbusClient.h"
#include <QDebug>
#include <bitset>

ModbusClient::ModbusClient(const ModbusInitArguments &args) : args_(args)
{
    // 启动连接线程
    keepConnection();
}

ModbusClient::~ModbusClient()
{
    // 设置线程退出条件为false
    bConnected_.store(false, std::memory_order_release);
    bThreadHolder_.store(false, std::memory_order_release);

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
        std::vector<uint16_t> cacheBuffer;
        cacheBuffer.resize(normalCacheInfo_.size);
        std::vector<uint16_t> realtimeBuffer;
        realtimeBuffer.resize(realCacheInfo_.size);
        uint16_t offsetCount = 0;
        while (bThreadHolder_.load(std::memory_order_acquire))
        {
            if (bConnected_.load(std::memory_order_acquire))
            {
                // 缓存写
                while (!qWriteData_.empty())
                {
                    RegisterWriteData wData;
                    if (qWriteData_.dequeue(wData))
                    {
                        writeRegisters(wData.wStartAddr, wData.wSize, wData.wData);
                    }
                }
                // 读缓存
                if (offsetCount >= 100)
                {
                    if (readRegisters(normalCacheInfo_.address, normalCacheInfo_.size, cacheBuffer))
                    {
                        std::lock_guard lock(mtxReadCache_);
                        std::copy(cacheBuffer.begin(), cacheBuffer.end(), normalCacheInfo_.cache.begin());
                    }
                    offsetCount = 0;
                }
                if (readRegisters(realCacheInfo_.address, realCacheInfo_.size, realtimeBuffer))
                {
                    std::lock_guard lock(mtxRealtime_);
                    std::copy(realtimeBuffer.begin(), realtimeBuffer.end(), realCacheInfo_.cache.begin());
                }
                offsetCount++;
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
            else
            {
                std::this_thread::sleep_for(std::chrono::seconds(2));
            }
        }
    }));
}

void ModbusClient::addNormalCache(uint16_t addr, uint16_t size)
{
    normalCacheInfo_.size = size;
    normalCacheInfo_.address = addr;
    normalCacheInfo_.cache.resize(size);
}

void ModbusClient::addRealtimeCache(uint16_t addr, uint16_t size)
{
    realCacheInfo_.size = size;
    realCacheInfo_.address = addr;
    realCacheInfo_.cache.resize(size);
}

void ModbusClient::addWriteCache(uint16_t addr, uint16_t size)
{
    writeCacheInfo_.address = addr;
    writeCacheInfo_.size = size;
    writeCacheInfo_.cache.resize(size);
}

bool ModbusClient::readCache(uint16_t address, uint16_t count, std::vector<uint16_t> &outData)
{
    // 检查是否越界
    uint16_t addr = address - 1 - normalCacheInfo_.address;
    if (addr + count > normalCacheInfo_.cache.size())
    {
        // 如果请求的范围超过了缓存的容量，返回一个空的结果向量
        return false;
    }
    // 使用std::copy进行批量读取
    std::lock_guard lock(mtxReadCache_); // 加缓存读锁
    std::copy(normalCacheInfo_.cache.begin() + addr, normalCacheInfo_.cache.begin() + addr + count, outData.begin());

    return true;
}

CacheInfo ModbusClient::readRealtimeInfo()
{
    std::lock_guard lock(mtxRealtime_);
    return realCacheInfo_;
}

bool ModbusClient::writeCache(uint16_t address, WriteRegisterType type, const uint16_t *data)
{
    bool ret = true;
    address -= 1;
    uint16_t cacheIndex = address - writeCacheInfo_.address;
    if (cacheIndex < 0)
    {
        return false;
    }
    switch (type)
    {
    case WriteRegisterType::RegBool: {
        auto tempVal = std::bitset<16>(writeCacheInfo_.cache[cacheIndex]);
        tempVal.set(data[0], data[1]);
        uint16_t uint16Val = tempVal.to_ulong();
        writeCacheInfo_.cache[cacheIndex] = uint16Val;
        qWriteData_.enqueue(RegisterWriteData(address, uint16Val));
        break;
    }
    case WriteRegisterType::RegInt: {
        writeCacheInfo_.cache[cacheIndex] = data[0];
        qWriteData_.enqueue(RegisterWriteData(address, data[0]));
        break;
    }
    case WriteRegisterType::RegReal: {
        writeCacheInfo_.cache[cacheIndex] = data[0];
        writeCacheInfo_.cache[cacheIndex + 1] = data[1];
        qWriteData_.enqueue(RegisterWriteData(address, data));
        break;
    }
    case WriteRegisterType::RegDInt: {
        writeCacheInfo_.cache[cacheIndex] = data[0];
        writeCacheInfo_.cache[cacheIndex + 1] = data[1];
        qWriteData_.enqueue(RegisterWriteData(address, data));
        break;
    }
    default:
        break;
    }
    return ret;
}

bool ModbusClient::getConnection()
{
    return bConnected_.load(std::memory_order_acquire);
}

bool ModbusClient::readRegisters(uint16_t address, uint16_t count, std::vector<uint16_t> &cache)
{
    bool ret = true;
    uint16_t remainingCount = count;
    uint16_t currentAddress = address;
    uint16_t batchCount = std::min(remainingCount, static_cast<uint16_t>(100)); // Max batch size 100

    while (remainingCount > 0)
    {
        int result =
            modbus_read_registers(mbsContext_, currentAddress, batchCount, cache.data() + (count - remainingCount));

        if (result == -1)
        {
            LogError("Failed to read registers={}, size={}", currentAddress, batchCount);
            bConnected_.store(false, std::memory_order_release);
            cvConnector_.notify_all();
            ret = false;
            break;
        }

        remainingCount -= batchCount;
        currentAddress += batchCount;
        batchCount = std::min(remainingCount, static_cast<uint16_t>(100));
    }

    return ret;
}

bool ModbusClient::writeRegisters(uint16_t address, uint16_t size, const uint16_t *values)
{
    bool ret = true;
    if (modbus_write_registers(mbsContext_, address, size, values) == -1)
    {
        LogError("Failed to write registers");
        bConnected_.store(false, std::memory_order_release);
        cvConnector_.notify_all();
        ret = false;
    }
    return ret;
}

void ModbusClient::keepConnection()
{
    tasks_.emplace_back(std::thread([this]() {
        while (bThreadHolder_.load(std::memory_order_acquire))
        {
            std::unique_lock<std::mutex> lock(mtxMbs_);

            // 等待连接断开或者退出条件
            cvConnector_.wait(lock, [this] { return bConnected_.load(std::memory_order_acquire) == false; });

            // 检查线程退出条件
            if (bThreadHolder_.load(std::memory_order_acquire) == false)
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
                // LogError("connect to PLC faile.");
                continue;
            }
            LogInfo("PLC connect success");
            // 设置响应超时时间
            struct timeval tv;
            tv.tv_sec = 0;
            tv.tv_usec = 2000000;
            modbus_set_response_timeout(mbsContext_, tv.tv_sec, tv.tv_usec);
            bConnected_.store(true, std::memory_order_release);
        }
    }));
}
