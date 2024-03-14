#pragma once
#include "BaseClient.h"
#include <QObject>
#include <cstdint>
#include <mutex>
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

enum class PLCStateType
{
    PLCStates = 0,
    PLCSteps = 1
};

enum class PLCCount
{
    InCount = 0,
    OutCount = 1,
    CameraCount = 2,
    QRCodeCount = 4,
    CodeCount
};

class BasePLCDevice : public QObject
{
    Q_OBJECT
  public:
    explicit BasePLCDevice(QObject *parent = nullptr);
    virtual ~BasePLCDevice();
    virtual bool init(const YAML::Node &config, const QString &path);
    virtual void zeroClear();               // 清零PLC计数
    virtual uint32_t getPlcCount(PLCCount); // 获取plc计数
    bool isConnected();
    uint8_t getDeviceStatus(PLCStateType states);
    // 批量数值读写
    // 支持float/uint16_t/uint32_t/uint8_t
    // 其中uint8_t用于存储单个二进制位bit，多少个uint8_t就是多少位
    // 因为vector<bool>本身存在的存储结构问题和性能缺少，所以采用vector<uint8_t>替代

    // addr: 读写起始地址，PLC地址：如40001输入参数 = 1;
    // bit: 读写的bit位，0~15;
    // flip: 是否翻转高低16位，默认不翻转。只有float/uint32_t支持翻转读写
    // size: 需要读写数据的个数

    // 用于读写bit值
    uint8_t readBitValue(const uint16_t addr, const uint16_t bitPos);
    // 单个完整值（byte, word, float）读取
    std::string readDevice(const std::string &readParams);
    template <typename ValueType> ValueType readSingle(const uint16_t addr)
    {
        auto reads = readArray<ValueType>(addr, 1);
        if (reads.size() > 0)
        {
            return reads[0];
        }
        return ValueType{};
    }

    // 单写
    bool writeDevice(const std::string &key, const std::string &value);
    bool writeBitValue(const uint16_t dbNmuber, const uint16_t addr, const uint16_t bitPos, uint8_t value);
    template <typename ValueType> bool writeSingle(const uint16_t dbNmuber, const uint16_t addr, ValueType value)
    {
        return writeArray<ValueType>(dbNmuber, addr, std::vector<ValueType>{value});
    }

    // 多读
    template <typename ValueType> std::vector<ValueType> readArray(const uint16_t addr, const uint16_t size);
    // 多写
    template <typename ValueType>
    bool writeArray(const uint16_t dbNmuber, const uint16_t addr, const std::vector<ValueType> &values);
  signals:
    void signalProductIn(const uint32_t count);  // 进料信号 count是需要创建的个数 大于等于1
    void signalProductOut(const uint32_t count); // 出料信号
    void signalQR();     // 扫码信号 针对回转式而言 此信号相当于原bottlemove信号
    void signalCoding(); // 打码信号
    void signalOCR();    // OCR信号
    void signalLocate(); // 定位相机信号
    void signalCheck();  // 复核相机信号

    void signalStatus(const uint8_t status); // 状态改变信号
    void signalSteps(const uint8_t steps);   // 设备步骤改变信号

  protected:
    BasePLCDevice(const BasePLCDevice &) = delete;
    BasePLCDevice &operator=(const BasePLCDevice &) = delete;
    // 通用保护接口，无需子类化
    bool initAddress(const QString &path);
    bool initAlert(const QString &path);
    void updateCaches(uint16_t address, const std::vector<uint8_t> &candidates);
    // 以下接口子类化
    virtual bool afterInit() = 0;
    virtual void heartbeat() = 0;
    virtual void closeProductionLine() = 0;
    virtual void updateRealTimeInfo() = 0;
    virtual void parsingAlertInfo() = 0;
    virtual void parsingStatusInfo() = 0;
    virtual void parsingPLCIDInfo() = 0;
    // 数据成员
    std::unique_ptr<BaseClient> client_{nullptr};
    DeviceType devType;

    std::atomic_bool updateHolder_{true};
    std::thread thUpdate_;

    bool bytesFlip_ = false;
    uint16_t cacheAddress_ = 0;
    std::mutex mtxCaches_;
    std::vector<uint8_t> caches_;
    uint8_t plcStates_ = 0;
    uint8_t plcSteps_ = 0;
    uint32_t prudctInNumber_ = 0;
    uint32_t prudctOutNumber_ = 0;
    uint32_t productCameraNumber_ = 0;
    uint32_t productQRCodeNumber_ = 0;
    uint32_t productCodeNumber_ = 0;
    uint8_t bHeartbeat_ = 0;
    // alert map
    std::unordered_map<std::string, std::string> mapAlertStore_;
    std::map<std::string, std::string> mapRecordAlert_;
    // address map
    std::string protocol_{};
    // key from qml
    // vector长度为4
    // [0]位db块编号：0（无效），有效值为编号值
    // [1]位地址码：值为地址编号
    // [2]位bit位：0~15有效
    // [3]位数据类型：0（无效），1（bool）,2(int),3(real)
    std::unordered_map<std::string, std::vector<uint16_t>> mapAddressInfo_;
};

template <typename ValueType>
inline std::vector<ValueType> BasePLCDevice::readArray(const uint16_t addr, const uint16_t size)
{
    static_assert(std::is_same_v<ValueType, float> || std::is_same_v<ValueType, uint16_t> ||
                      std::is_same_v<ValueType, uint32_t> || std::is_same_v<ValueType, uint8_t>,
                  "readDevices only supports float, uint16_t, uint8_t");

    if constexpr (std::is_same_v<ValueType, uint8_t>)
    {
        std::vector<uint8_t> retVec;

        // 计算起始索引和结束索引
        size_t startIndex = addr - cacheAddress_;
        size_t endIndex = std::min(startIndex + size, caches_.size());
        // 使用 std::copy_if 将数据从缓存中复制到返回向量中
        std::lock_guard lock(mtxCaches_);
        std::copy_if(caches_.begin() + startIndex, caches_.begin() + endIndex, std::back_inserter(retVec),
                     [](uint8_t value) { return true; }); // 如果你有其他过滤条件，可以在这里添加
        return retVec;
    }
    else if constexpr (std::is_same_v<ValueType, uint16_t>)
    {
        std::vector<uint8_t> retBytes = readArray<uint8_t>(addr, size * 2); // 乘以 2 获取字节数量
        std::vector<uint16_t> values;

        // 按照每两个字节高低翻转的顺序组合成 uint16_t 值，并放入 values 中
        for (size_t i = 0; i < retBytes.size(); i += 2)
        {
            size_t low = i;
            size_t high = i + 1;
            if (bytesFlip_)
            {
                std::swap(low, high);
            }
            uint16_t value = retBytes[low] | (retBytes[high] << 8);
            values.push_back(value);
        }

        return values;
    }
    else if constexpr (std::is_same_v<ValueType, uint32_t>)
    {
        std::vector<uint16_t> retWords = readArray<uint16_t>(addr, size * 2);
        std::vector<uint32_t> values;

        // 按照每两个字节高低翻转的顺序组合成 uint32_t 值，并放入 values 中
        for (size_t i = 0; i < retWords.size(); i += 2)
        {
            size_t low = i;
            size_t high = i + 1;
            if (bytesFlip_)
            {
                std::swap(low, high);
            }
            uint32_t value = retWords[low] | (retWords[high] << 16);
            values.push_back(value);
        }

        return values;
    }
    else if constexpr (std::is_same_v<ValueType, float>)
    {
        std::vector<uint32_t> retDInts = readArray<uint32_t>(addr, size * 2);
        std::vector<float> values;
        for (const auto &temp : retDInts)
        {
            values.push_back(temp / 100.0f);
        }
        return values;
    }
}

template <typename ValueType>
bool BasePLCDevice::writeArray(const uint16_t dbNumber, const uint16_t addr, const std::vector<ValueType> &values)
{
    static_assert(std::is_same_v<ValueType, float> || std::is_same_v<ValueType, uint16_t> ||
                      std::is_same_v<ValueType, uint32_t> || std::is_same_v<ValueType, uint8_t>,
                  "writeDevices only supports float,uint8_t or uint16_t");
    if (client_ == nullptr)
        return false;
    if constexpr (std::is_same_v<ValueType, float>)
    {
        return client_->writeFloats(dbNumber, addr, values);
    }
    else if constexpr (std::is_same_v<ValueType, uint16_t>)
    {
        return client_->writeWords(dbNumber, addr, values);
    }
    else if constexpr (std::is_same_v<ValueType, uint32_t>)
    {
        return client_->writeDInts(dbNumber, addr, values);
    }
    else if constexpr (std::is_same_v<ValueType, uint8_t>)
    {
        return client_->writeBytes(dbNumber, addr, values);
    }
    return false;
}