#pragma once
#include "ModbusClient.h"
#include "RegistersWapper.h"
#include <iostream>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>
class PLCDevice
{
  public:
    explicit PLCDevice();
    ~PLCDevice();
    void init();
    bool writeDataToDevice(std::string addr, std::string type, std::string value);

  protected:
    PLCDevice(const PLCDevice &) = delete;
    PLCDevice &operator=(const PLCDevice &) = delete;
    PLCDevice(PLCDevice &&) noexcept(true) = default;
    PLCDevice &operator=(PLCDevice &&) noexcept(true) = default;
    void updateData();
    void alertParsing(const uint16_t *alertGoup, uint16_t start, uint16_t plcAddr);

    void motorBoolParsing(const uint16_t *alertGoup, uint16_t start, uint16_t plcAddr);

    void sensorParsing(const uint16_t *alertGoup, uint16_t start, uint16_t plcAddr);

    void volveParsing(const uint16_t *alertGoup, uint16_t start, uint16_t plcAddr);

    void produceRealParsing(const uint16_t *alertGoup, uint16_t start, uint16_t plcAddr);

    void produceIntParsing(const uint16_t *alertGoup, uint16_t start, uint16_t plcAddr);

    void produceWordParsing(const uint16_t *alertGoup, uint16_t start, uint16_t plcAddr);

    void fifoBoolParsing(const uint16_t *alertGoup, uint16_t start, uint16_t plcAddr);

    void motorRealParsing(const uint16_t *alertGoup, uint16_t start, uint16_t plcAddr);

    void fifoIntParsing(const uint16_t *alertGoup, uint16_t start, uint16_t plcAddr);

  private:
    RegistersWapper regWapper_;
    std::atomic_bool updateHolder_{true};
    std::thread thUpdate_;
    ModbusClient *client_ = nullptr;
};