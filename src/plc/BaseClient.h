#pragma once
#include <cstdint>
#include <vector>

class BaseClient
{
  public:
    BaseClient() = default;
    virtual ~BaseClient() = default;
    BaseClient(const BaseClient &) = delete;
    BaseClient &operator=(const BaseClient &) = delete;
    virtual bool isConnected() = 0;
    virtual bool readBit(const uint16_t dbNumber, const uint16_t address, const uint16_t bitPos, uint8_t &value) = 0;
    virtual bool readBytes(const uint16_t dbNumber, const uint16_t address, std::vector<uint8_t> &values) = 0;
    virtual bool readWords(const uint16_t dbNumber, const uint16_t address, std::vector<uint16_t> &values) = 0;
    virtual bool readDInts(const uint16_t dbNumber, const uint16_t address, std::vector<uint32_t> &values) = 0;
    virtual bool readFloats(const uint16_t dbNumber, const uint16_t address, std::vector<float> &values) = 0;
    virtual bool writeBit(const uint16_t dbNumber, const uint16_t address, const uint16_t bitPos,
                          const uint8_t value) = 0;
    virtual bool writeBytes(const uint16_t dbNumber, const uint16_t address, const std::vector<uint8_t> &values) = 0;
    virtual bool writeWords(const uint16_t dbNumber, const uint16_t address, const std::vector<uint16_t> &values) = 0;
    virtual bool writeDInts(const uint16_t dbNumber, const uint16_t address, const std::vector<uint32_t> &values) = 0;
    virtual bool writeFloats(const uint16_t dbNumber, const uint16_t address, const std::vector<float> &values) = 0;

  protected:
    virtual void keepConnection() = 0;
};
