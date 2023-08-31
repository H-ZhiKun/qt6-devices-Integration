#pragma once
#include "ModbusClient.h"
#include "RegistersWapper.h"
#include <QObject>
#include <iostream>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>
struct FIFOInfo
{
    uint16_t numQRCode;          // 412643 二维码读取工位小瓶编号
    uint16_t numPosition;        // 412644 定位拍照工位小瓶编号
    uint16_t numVerifyPos;       // 412645 定位确认拍照工位小瓶编号
    uint16_t numCoding;          // 412646 喷码工位小瓶编号
    uint16_t numVerifyCoding;    // 412647 喷码复核工位小瓶编号
    uint16_t signalMove;         // 412654 喷码复核工位小瓶编号
    uint16_t signalSearchCoding; // 412655 喷码数据查找信号
};

class PLCDevice : public QObject
{
    Q_OBJECT
  public:
    explicit PLCDevice(QObject *parent = nullptr);
    ~PLCDevice();
    void init(const std::string &host, uint16_t port, uint16_t ioFreq, uint16_t FIFOFreq);
    std::string readDevice(const std::string &type, const std::string &addr, const std::string &bit = "");
    bool writeDataToDevice(const std::string &type, const std::string &addr, const std::string &bit = "",
                           const std::string &value = "");
    const FIFOInfo &getFIFOInfo();
    bool getConnect();

  protected:
    PLCDevice(const PLCDevice &) = delete;
    PLCDevice &operator=(const PLCDevice &) = delete;
    PLCDevice(PLCDevice &&) noexcept(true) = default;
    PLCDevice &operator=(PLCDevice &&) noexcept(true) = default;
    void updateData();
    void alertParsing(const uint16_t *alertGroup, uint16_t size);
    void FIFOParsing(const uint16_t *FIFOGroup, uint16_t size);
  signals:
    void bottomMove(const uint64_t);        // 瓶位移动信号
    void readQRCode(const uint64_t);        // 二维码读取信号
    void locatePhoto(const uint64_t);      // 定位拍照信号
    void locateCheckPhoto(const uint64_t); // 定位复核拍照信号
    void codeLogistics(const uint64_t);     // 打码信号
    void codeCheck(const uint64_t);        // 打码复核信号
    void codeSerch(const uint64_t);         // 喷码数据查找信号
  private:
    RegistersWapper regWapper_;
    std::atomic_bool updateHolder_{true};
    std::thread thUpdate_;
    ModbusClient *client_ = nullptr;
    const uint16_t readBeginAddress_ = 12288;
    const uint16_t readCacheSize_ = 331;
    const uint16_t writeBeginAddress_ = 12688;
    const uint16_t writeCacheSize_ = 316;
    const uint16_t FIFOBeginAddress_ = 12641;
    const uint16_t FIFOCacheSize_ = 14;
    FIFOInfo fifoInfo_;
};