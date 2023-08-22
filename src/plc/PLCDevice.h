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
    void alertParsing(const uint16_t *alertGoup, uint16_t start);

    void motorBoolParsing(const uint16_t *alertGoup, uint16_t start);

    void sensorParsing(const uint16_t *alertGoup, uint16_t start);

    void volveParsing(const uint16_t *alertGoup, uint16_t start);

    void produceRealParsing(const uint16_t *alertGoup, uint16_t start);

    void produceIntParsing(const uint16_t *alertGoup, uint16_t start);

    void produceWordParsing(const uint16_t *alertGoup, uint16_t start);

    void fifoBoolParsing(const uint16_t *alertGoup, uint16_t start);

    void motorRealParsing(const uint16_t *alertGoup, uint16_t start);

    void fifoIntParsing(const uint16_t *alertGoup, uint16_t start);

  private:
    RegistersWapper regWapper_;
    std::atomic_bool updateHolder_{true};
    std::thread thUpdate_;
    ModbusClient *client_ = nullptr;
    uint16_t addrStartPLC_ = 0;
};