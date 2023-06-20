#include "ModbusClient.h"

ModbusClient::ModbusClient(const std::string &ip, uint16_t port)
    : ip_(ip), port_(port)
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
        it.join();
}

void ModbusClient::work(const std::vector<ModbusReadArgument> &startArgs)
{
    // 启动连接线程
    keepConnection();

    // 根据启动参数列表创建任务线程
    for (const auto &arg : startArgs)
    {
        tasks_.emplace_back(std::thread([arg, this]()
        {
            std::vector<uint16_t> readBuffer;
            readBuffer.resize(arg.offset);

            while (bThreadHolder_)
            {
                if (bThreadHolder_ == false)
                    break;

                if (bConnected_)
                {
                    readRegisters(arg.addr, arg.offset, readBuffer);
                    updateCache(arg.addr, readBuffer);
                    std::this_thread::sleep_for(std::chrono::milliseconds(arg.clock));
                }
                else
                {
                    std::this_thread::sleep_for(std::chrono::seconds(2));
                }
            }
        }));
    }
}

std::vector<uint16_t> ModbusClient::readDatas(uint16_t address, uint16_t count)
{
    std::shared_lock<std::shared_mutex> lock(mtxCache_); // 加缓存读锁



    std::vector<uint16_t> res;
    for (int i = 0; i < count; i++)
    {
        auto iter = cache_.find(address + i);
        if (iter != cache_.end())
        {
            res.emplace_back(iter->second);
        }
    }
    return res;
}

void ModbusClient::writeDatas(uint16_t address, const std::vector<uint16_t> &values)
{
    writeRegisters(address, values);
}

bool ModbusClient::getConnection()
{
    return bConnected_;
}

void ModbusClient::readRegisters(uint16_t address, uint16_t count, std::vector<uint16_t> &buffer)
{
    std::lock_guard<std::mutex> lock(mtxMbs_);
    if (modbus_read_registers(mbsContext_, address, count, buffer.data()) == -1)
    {
        LOGERROR("Failed to read registers");
        bConnected_ = false;
        cvConnector_.notify_all();
    }
}

void ModbusClient::writeRegisters(uint16_t address, const std::vector<uint16_t> &values)
{
    std::lock_guard<std::mutex> lock(mtxMbs_);
    if (modbus_write_registers(mbsContext_, address, values.size(), values.data()) == -1)
    {
        LOGERROR("Failed to write registers");
        bConnected_ = false;
        cvConnector_.notify_all();
    }
}

void ModbusClient::updateCache(uint16_t address, const std::vector<uint16_t> &values)
{
    std::unique_lock<std::shared_mutex> lock(mtxCache_); // 加缓存写锁
    for (size_t i = 0; i < values.size(); ++i)
    {
        cache_[address] = values[i];
        ++address;
    }
}

void ModbusClient::keepConnection()
{
    tasks_.emplace_back(std::thread([this]()
    {
        while (bThreadHolder_)
        {
            std::unique_lock<std::mutex> lock(mtxMbs_);

            // 等待连接断开或者退出条件
            cvConnector_.wait(lock, [this] {
                return bConnected_ == false;
            });

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
