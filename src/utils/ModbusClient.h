#pragma once
#include "LockFreeQueue.h"
#include "Logger.h"
#include "modbus.h"
#include <string>
#include <unordered_map>
#include <vector>

/**
 * @brief Modbus初始化参数
 */

class ModbusInitArguments
{
  public:
    std::string ip; // plc ip地址
    uint16_t port;  // plc 端口号
};

class RegisterWriteData
{
  public:
    RegisterWriteData()
    {
    }
    RegisterWriteData(uint16_t addr, const uint16_t data)
    {
        wStartAddr = addr;
        wData[0] = data;
    }
    RegisterWriteData(uint16_t addr, const uint16_t *data)
    {
        wStartAddr = addr;
        wData[0] = data[0];
        wData[1] = data[1];
        wSize = 2;
    }
    uint16_t wStartAddr = 0; /**< 写寄存器头地址 */
    uint16_t wSize = 1;      // 写入长度
    uint16_t wData[2]{0};    // 写入数据
};

class CacheInfo
{
  public:
    uint16_t address = 0;        /**< 寄存器头地址 */
    uint16_t size = 0;           /**< 寄存器数量 */
    std::vector<uint16_t> cache; // 寄存器缓存
};

enum class WriteRegisterType
{
    RegBool,
    RegInt,
    RegReal
};

/**
 * @brief Modbus客户端类
 */
class ModbusClient
{
  public:
    /**
     * @brief 构造函数，创建ModbusClient对象
     * @param ip Modbus设备的IP地址
     * @param port Modbus设备的端口号
     */
    explicit ModbusClient(const ModbusInitArguments &args);

    /**
     * @brief 析构函数，释放Modbus上下文
     */
    virtual ~ModbusClient();

    /**
     * @brief 启动ModbusClient，开始读取数据
     * @param startArgs 启动参数列表
     */
    ModbusClient(const ModbusClient &) = delete;
    ModbusClient &operator=(const ModbusClient &) = delete;
    ModbusClient(ModbusClient &&) noexcept(true) = default;
    ModbusClient &operator=(ModbusClient &&) noexcept(true) = default;
    void work();
    void addNormalCache(uint16_t addr, uint16_t size);
    void addRealtimeCache(uint16_t addr, uint16_t size);
    void addWriteCache(uint16_t addr, uint16_t size);
    /**
     * @brief 读取寄存器数据
     * @param address 寄存器起始地址
     * @param count 读取的寄存器数量
     * @return 读取的寄存器数据
     */
    bool readCache(uint16_t address, uint16_t count, std::vector<uint16_t> &outData);
    CacheInfo readRealtimeInfo();
    /**
     * @brief 写入寄存器数据
     * @param address 寄存器起始地址
     * @param values 要写入的寄存器值
     */
    bool writeCache(uint16_t address, WriteRegisterType type, const uint16_t *data);

    /**
     * @brief 获取当前连接状态
     * @return 返回连接状态
     */
    bool getConnection();

  private:
    /**
     * @brief 读取寄存器数据
     * @param address 寄存器起始地址
     * @param count 读取的寄存器数量
     * @param buffer 读取结果缓冲区
     */
    bool readRegisters(uint16_t address, uint16_t count, std::vector<uint16_t> &cache);

    /**
     * @brief 写入寄存器数据
     * @param address 寄存器起始地址
     * @param values 要写入的寄存器值
     */
    bool writeRegisters(uint16_t address, uint16_t size, const uint16_t *values);

  private:
    modbus_t *mbsContext_ = nullptr;              /**< Modbus上下文指针 */
    std::mutex mtxMbs_;                           /**< 互斥锁，用于保护Modbus访问 */
    std::mutex mtxReadCache_;                     // 读缓存锁
    std::mutex mtxRealtime_;                      // 实时数据缓存锁
    std::vector<std::thread> tasks_;              // 存储当前任务线程。
    std::atomic_bool bThreadHolder_{true};        // 子线程保持者，在析构中退出。
    std::condition_variable cvConnector_;         // 重连线程条件变量
    std::atomic_bool bConnected_{false};          // 当前连接设备状态
    ModbusInitArguments args_;                    // 初始化参数 只读
    CacheInfo normalCacheInfo_;                   // 读缓存数据结构
    CacheInfo realCacheInfo_;                     // FIFO读缓存数据结构
    CacheInfo writeCacheInfo_;                    // 写缓存数据结构
    LockFreeQueue<RegisterWriteData> qWriteData_; // 写寄存器执行队列

    /**
     * @brief 自动重连到Modbus设备
     */
    void keepConnection();
};
