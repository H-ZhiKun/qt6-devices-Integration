#pragma once
#include "BaseClient.h"
#include "snap7.h"
#include <atomic>
#include <cstdint>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

/**
 * @brief Modbus客户端类
 */
class Snap7Client : public BaseClient
{
  public:
    explicit Snap7Client(const std::string &ip);
    virtual ~Snap7Client();
    virtual bool isConnected() override;
    // 注意readBits(writeBits)的参数和readBytes(writeBytes)的参数都是vector<uint8_t>
    // 前者是代表二进制位的个数，后者是字节个数
    virtual bool readBit(const uint16_t dbNumber, const uint16_t address, const uint16_t bitPos,
                         uint8_t &value) override;

    virtual bool readBytes(const uint16_t dbNumber, const uint16_t address, std::vector<uint8_t> &values) override;
    virtual bool readWords(const uint16_t dbNumber, const uint16_t address, std::vector<uint16_t> &values) override;
    virtual bool readDInts(const uint16_t dbNumber, const uint16_t address, std::vector<uint32_t> &values) override;
    virtual bool readFloats(const uint16_t dbNumber, const uint16_t address, std::vector<float> &values) override;

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
    std::mutex mtxContext_;
    uint16_t rack_ = 0;                    // 机架号 默认0
    uint16_t slot_ = 0;                    // 槽号 默认0
    TS7Client context_;                    /**< Modbus上下文指针 */
    std::thread thKeepConnection_;         // 保持连接线程。
    std::atomic_bool bThreadHolder_{true}; // 线程保持者，在析构中退出。
    std::condition_variable cvConnector_;  // 重连线程条件变量
    std::atomic_bool bConnected_{false};   // 当前连接设备状态
};
