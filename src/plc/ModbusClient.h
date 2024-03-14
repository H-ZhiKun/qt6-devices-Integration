#pragma once
#include "BaseClient.h"
#include <atomic>
#include <cstdint>
#include <modbus.h>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

/**
 * @brief Modbus客户端类
 */
class ModbusClient : public BaseClient
{
  public:
    /**
     * @brief 构造函数，创建ModbusClient对象
     * @param ip Modbus设备的IP地址
     * @param port Modbus设备的端口号
     */
    explicit ModbusClient(const std::string &ip, const uint16_t port);

    /**
     * @brief 析构函数，释放Modbus上下文
     */
    virtual ~ModbusClient();
    virtual bool isConnected() override;
    /**
     * @brief 读取寄存器数据
     * @param address 寄存器起始地址
     * @param count 读取的寄存器数量
     * @param buffer 读取结果缓冲区
     */
    virtual bool readBit(const uint16_t dbNumber, const uint16_t address, const uint16_t bitPos,
                         uint8_t &value) override;
    virtual bool readBytes(const uint16_t dbNumber, const uint16_t address, std::vector<uint8_t> &values) override;
    virtual bool readWords(const uint16_t dbNumber, const uint16_t address, std::vector<uint16_t> &values) override;
    virtual bool readDInts(const uint16_t dbNumber, const uint16_t address, std::vector<uint32_t> &values) override;
    virtual bool readFloats(const uint16_t dbNumber, const uint16_t address, std::vector<float> &values) override;
    /**
     * @brief 写入寄存器数据
     * @param address 寄存器起始地址
     * @param values 要写入的寄存器值
     */
    virtual bool writeBit(const uint16_t dbNumber, const uint16_t address, const uint16_t bitPos,
                          const uint8_t value) override;

    virtual bool writeBytes(const uint16_t dbNumber, const uint16_t address,
                            const std::vector<uint8_t> &values) override;
    virtual bool writeWords(const uint16_t dbNumber, const uint16_t address,
                            const std::vector<uint16_t> &values) override;
    virtual bool writeDInts(const uint16_t dbNumber, const uint16_t address,
                            const std::vector<uint32_t> &values) override;
    virtual bool writeFloats(const uint16_t dbNumber, const uint16_t address,
                             const std::vector<float> &values) override;

  protected:
    /**
     * @brief 自动重连到Modbus设备
     */
    virtual void keepConnection() override;
    std::string ip_;
    uint16_t port_;
    modbus_t *mbsContext_ = nullptr;       /**< Modbus上下文指针 */
    std::mutex mtxMbs_;                    /**< 互斥锁，用于保护Modbus访问 */
    std::thread thKeepConnection_;         // 保持连接线程。
    std::atomic_bool bThreadHolder_{true}; // 线程保持者，在析构中退出。
    std::condition_variable cvConnector_;  // 重连线程条件变量
    std::atomic_bool bConnected_{false};   // 当前连接设备状态
};
