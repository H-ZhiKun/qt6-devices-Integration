#pragma once
#include "ModbusClient.h"
#include "RegistersWapper.h"
#include <QObject>
#include <iostream>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>
#include <yaml-cpp/yaml.h>
enum class DeviceType
{
    CircleDevice,
    LineDevice,
    CapDevice
};

struct FIFOInfo
{
    uint16_t numQRCode;          // 412643 二维码读取工位小瓶编号
    uint16_t numPosition;        // 412644 定位拍照工位小瓶编号
    uint16_t numVerifyPos;       // 412645 定位确认拍照工位小瓶编号
    uint16_t numCoding;          // 412646 喷码工位小瓶编号
    uint16_t numVerifyCoding;    // 412647 喷码复核工位小瓶编号
    uint16_t signalMove;         // 412654 工位信号
    uint16_t signalSearchCoding; // 412655 喷码数据查找信号
};

struct LineInfo
{
    uint16_t sigCoding; // 40112 打码信号
    uint16_t sigCognex; // 40113 读码信号
};

struct CapInfo
{
    uint16_t sigCognex; // 40113 读码信号
};

class PLCDevice : public QObject
{
    Q_OBJECT
  public:
    explicit PLCDevice(QObject *parent = nullptr);
    ~PLCDevice();
    void init(const YAML::Node &config);
    std::string readDevice(const std::string &type, const std::string &addr, const std::string &bit = "");
    bool writeDevice(const std::string &type, const std::string &addr, const std::string &bit = "",
                     const std::string &value = "");
    const FIFOInfo &getFIFOInfo();
    bool getConnect();
  signals:
    // 回转式信号
    void bottleMove(const uint64_t); // 瓶位移动信号
    // 直线打码信号
    void lineCognex(); // 直线扫码信号
    void lineCoding(); // 直线打码信号
    // 瓶盖信号
    void capCognex(); // 瓶盖扫码信号
  protected:
    PLCDevice(const PLCDevice &) = delete;
    PLCDevice &operator=(const PLCDevice &) = delete;
    PLCDevice(PLCDevice &&) noexcept(true) = default;
    PLCDevice &operator=(PLCDevice &&) noexcept(true) = default;
    // circle function begin
    void updateReadInfo();
    void alertParsing(const uint16_t *alertGroup, uint16_t size);
    void circleAlertParing(const uint16_t *alertGroup, uint16_t size);
    void realParsing(const uint16_t *realGroup, uint16_t size);
    void FIFOParsing(const uint16_t *FIFOGroup, uint16_t size);
    void lineParsing(const uint16_t *lineGroup, uint16_t size);
    void capParsing(const uint16_t *capGroup, uint16_t size);

  private:
    RegistersWapper regWapper_;
    std::atomic_bool updateHolder_{true};
    std::thread thUpdate_;
    ModbusClient *client_ = nullptr;
    uint16_t readBeginAddress_ = 0;
    uint16_t readCacheSize_ = 0;
    uint16_t writeBeginAddress_ = 0;
    uint16_t writeCacheSize_ = 0;
    uint16_t realtimeBeginAddress_ = 0;
    uint16_t realtimeCacheSize_ = 0;
    FIFOInfo fifoInfo_;
    LineInfo lineInfo_;
    CapInfo capInfo_;
    DeviceType devType;
};