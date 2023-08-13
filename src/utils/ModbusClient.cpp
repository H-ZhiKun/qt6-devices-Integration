#include "ModbusClient.h"
#include <QDebug>

ModbusClient::ModbusClient(const std::string &ip, uint16_t port) : ip_(ip), port_(port)
{
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

void ModbusClient::work(ModbusReadArgument &&args)
{
    // 启动连接线程
    keepConnection();

    // 预留足够的缓存容量
    cache_.resize(args.offset);
    // 根据启动参数列表创建任务线程

    tasks_.emplace_back(std::thread([args, this]() {
        uint16_t count = args.offset / 100;
        uint16_t remainder = args.offset % 100;
        if (remainder > 0)
        {
            count++;
        }
        std::vector<uint16_t> readBuffer;
        readBuffer.resize(100);
        uint16_t currentAddr = 0;
        uint16_t currentOffset = 0;
        while (bThreadHolder_)
        {
            if (bConnected_)
            {
                currentAddr = args.addr;
                for (uint8_t i = 0; i < count; i++)
                {
                    if (i == count - 1)
                    {
                        currentOffset = remainder;
                    }
                    else
                    {
                        currentOffset = 100;
                    }
                    readRegisters(currentAddr, currentOffset);
                    currentAddr += 100;
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
    if (address + count > cache_.size())
    {
        // 如果请求的范围超过了缓存的容量，返回一个空的结果向量
        return false;
    }
    // 使用std::copy进行批量读取
    std::lock_guard lock(mtxMbs_); // 加缓存读锁
    std::copy(cache_.begin() + address, cache_.begin() + address + count, std::back_inserter(outData));

    return true;
}

void ModbusClient::writeDatas(uint16_t address, const std::vector<uint16_t> &values)
{
    writeRegisters(address, values);
}

bool ModbusClient::getConnection()
{
    return bConnected_;
}

bool ModbusClient::readRegisters(uint16_t address, uint16_t count)
{
    bool ret = true;
    std::lock_guard<std::mutex> lock(mtxMbs_);
    int result = modbus_read_registers(mbsContext_, address, count, cache_.data() + address);
    if (result == -1)
    {
        LogError("Failed to read registers");
        bConnected_ = false;
        cvConnector_.notify_all();
        ret = false;
    }
    else
    {
        qDebug() << fmt::format("updated by addr = {}, size = {}", address, result);
    }
    return ret;
}

bool ModbusClient::writeRegisters(uint16_t address, const std::vector<uint16_t> &values)
{
    bool ret = true;
    std::lock_guard<std::mutex> lock(mtxMbs_);
    if (modbus_write_registers(mbsContext_, address, values.size(), values.data()) == -1)
    {
        LogError("Failed to write registers");
        bConnected_ = false;
        cvConnector_.notify_all();
        ret = false;
    }
    return ret;
}

void ModbusClient::updateCache(uint16_t address, const std::vector<uint16_t> &values)
{
    std::lock_guard lock(mtxMbs_); // 加缓存写锁
    if (address + values.size() > cache_.size())
    {
        cache_.resize(values.size() + cache_.size());
    }
    std::copy(values.begin(), values.end(), cache_.begin() + address);
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
                break;

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
