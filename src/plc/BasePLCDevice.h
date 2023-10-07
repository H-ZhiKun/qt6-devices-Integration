#pragma once
#include "ModbusClient.h"
#include <QObject>
#include <string>
#include <thread>
#include <yaml-cpp/yaml.h>
enum class DeviceType
{
    CircleDevice,
    LineDevice,
    CapDevice
};

class BasePLCDevice : public QObject
{
    Q_OBJECT
  public:
    explicit BasePLCDevice(QObject *parent = nullptr);
    virtual ~BasePLCDevice();
    void init(const YAML::Node &config);
    std::string readDevice(const std::string &type, const std::string &addr, const std::string &bit = "");
    bool writeDevice(const std::string &type, const std::string &addr, const std::string &bit = "",
                     const std::string &value = "");
    bool getConnect();

  signals:

    void signalQR(const uint64_t); // 扫码信号 针对回转式而言 此信号相当于原bottlemove信号
    void signalCoding();           // 打码信号
    void signalOCR();              // OCR信号
    void signalRemove();           // 踢出信号

  protected:
    BasePLCDevice(const BasePLCDevice &) = delete;
    BasePLCDevice &operator=(const BasePLCDevice &) = delete;
    BasePLCDevice(BasePLCDevice &&) noexcept(true) = default;
    BasePLCDevice &operator=(BasePLCDevice &&) noexcept(true) = default;

    virtual void updateReadInfo();
    virtual void parsingReadInfo(const uint16_t *alertGroup, uint16_t size) = 0;
    virtual void parsingRealtimeInfo(const uint16_t *alertGroup, uint16_t size) = 0;

    std::atomic_bool updateHolder_{true};
    std::thread *thUpdate_;
    ModbusClient *client_ = nullptr;
    uint16_t readBeginAddress_ = 0;
    uint16_t readCacheSize_ = 0;
    uint16_t writeBeginAddress_ = 0;
    uint16_t writeCacheSize_ = 0;
    uint16_t realtimeBeginAddress_ = 0;
    uint16_t realtimeCacheSize_ = 0;
    DeviceType devType;
};