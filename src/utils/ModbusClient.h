#pragma once
#include <modbus/modbus.h>
#include <shared_mutex>
#include <unordered_map>
#include <string>
#include <vector>
#include "Logger.h"


/**
 * @brief Modbus读取参数
 */
class ModbusReadArgument
{
public:
    uint16_t addr = 0;   /**< 寄存器起始地址 */
    uint16_t offset = 0; /**< 寄存器数量 */
    uint8_t clock = 0;   /**< 读取间隔 */
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
    explicit ModbusClient(const std::string &ip, uint16_t port);

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
    void work(const std::vector<ModbusReadArgument> &startArgs);


    /**
     * @brief 读取寄存器数据
     * @param address 寄存器起始地址
     * @param count 读取的寄存器数量
     * @return 读取的寄存器数据
     */
    std::vector<uint16_t> readDatas(uint16_t address, uint16_t count = 1);

    /**
     * @brief 写入寄存器数据
     * @param address 寄存器起始地址
     * @param values 要写入的寄存器值
     */
    void writeDatas(uint16_t address, const std::vector<uint16_t> &values);

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
    void readRegisters(uint16_t address, uint16_t count, std::vector<uint16_t> &buffer);

    /**
     * @brief 写入寄存器数据
     * @param address 寄存器起始地址
     * @param values 要写入的寄存器值
     */
    void writeRegisters(uint16_t address, const std::vector<uint16_t> &values);

private:
    modbus_t *mbsContext_ = nullptr;               /**< Modbus上下文指针 */
    std::string ip_;                               /**< Modbus设备的IP地址 */
    uint16_t port_;                                /**< Modbus设备的端口号 */
    std::mutex mtxMbs_;                            /**< 互斥锁，用于保护Modbus访问 */
    std::shared_mutex mtxCache_;                   // 缓存读写锁
    std::unordered_map<uint16_t, uint16_t> cache_; /**< 寄存器值缓存 */
    std::vector<std::thread> tasks_;               // 存储当前任务线程。
    std::atomic_bool bThreadHolder_{true};         // 子线程保持者，在析构中退出。
    std::condition_variable cvConnector_;          // 重连线程条件变量
    std::atomic_bool bConnected_{false};           // 当前连接设备状态

    /**
     * @brief 更新寄存器值缓存
     * @param address 要更新的寄存器地址
     * @param values 对应的寄存器值列表
     */
    void updateCache(uint16_t address, const std::vector<uint16_t> &values);

    /**
     * @brief 自动重连到Modbus设备
     */
    void keepConnection();
};
