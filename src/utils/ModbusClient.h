#pragma once

#include <modbus/modbus.h>
#include <mutex>
#include <unordered_map>
#include <string>
#include <vector>
#include "Logger.h"

/**
 * @brief Modbus客户端类，用于与Modbus设备通信
 */
class ModbusClient {
public:
    /**
     * @brief 构造函数，创建ModbusClient对象
     * @param ipAddress Modbus设备的IP地址
     * @param port Modbus设备的端口号
     * @throws std::runtime_error 如果无法创建Modbus上下文或无法连接到Modbus设备时抛出异常
     */
    explicit ModbusClient(const std::string& ipAddress, int port);

    /**
     * @brief 析构函数，释放Modbus上下文
     */
    ~ModbusClient();

    /**
     * @brief 批量读取寄存器的值
     * @param addresses 要读取的寄存器地址列表
     * @return 读取到的寄存器值列表
     */
    std::vector<uint16_t> readRegisters(uint16_t address, uint16_t count = 1);

    /**
     * @brief 批量写入寄存器的值
     * @param address 要写入的寄存器首地址
     * @param values 要写入的寄存器值列表
     */
    void writeRegisters(uint16_t address, const std::vector<uint16_t>& values);

private:
    modbus_t* modbusContext; /**< Modbus上下文指针 */
    std::string ipAddress; /**< Modbus设备的IP地址 */
    int port; /**< Modbus设备的端口号 */
    std::mutex mutex; /**< 互斥锁，用于保护Modbus访问 */
    std::unordered_map<uint16_t, uint16_t> cache; /**< 寄存器值缓存 */

    /**
     * @brief 更新寄存器值缓存
     * @param addresses 要更新的寄存器地址列表
     * @param values 对应的寄存器值列表
     */
    void updateCache(const std::vector<uint16_t>& addresses, const std::vector<uint16_t>& values);

    /**
     * @brief 检查Modbus连接状态
     * @throws std::runtime_error 如果连接断开时抛出异常
     */
    void checkConnection();

    /**
     * @brief 自动重连到Modbus设备
     * @throws std::runtime_error 如果重新连接失败时抛出异常
     */
    void reconnect();

    /**
     * @brief 在连接断开时执行自动重连
     */
    void reconnectOnError();
};
