#pragma once
#include "AlertWapper.h"
#include "BaseClient.h"
#include <QObject>
#include <atomic>
#include <bitset>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <mutex>
#include <unordered_map>
#include <vector>
#include <yaml-cpp/yaml.h>

enum class DeviceUnion
{
    DevCircle = 0,
    DevLine,
    DevCap,
    In,
    Out,
    Qrcode,
    Print,
    Ocr,
    Location,
    LocateCheck,
    State,
    Step
};

class BasePLCDevice : public QObject
{
    Q_OBJECT
  public:
    explicit BasePLCDevice(QObject *parent = nullptr);
    virtual ~BasePLCDevice();
    bool init(const YAML::Node &config, const QString &path); // 清零PLC计数
    void setExperiment(bool inTest);
    bool isConnected();
    void keepConnection();
    uint32_t getDeviceData(const DeviceUnion wantKey);
    DeviceUnion getDeviceType();
    virtual bool fetchFast() = 0; // 快取寄存器
    virtual bool fetchLazy() = 0; // 慢速寄存器
    virtual bool fetchTest() = 0; // 测试寄存器
    virtual void heart() = 0;
    virtual bool getCollect(std::string windName)
    {
        return false;
    };
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

    // 报警解析
    template <typename ValueType> void parseAlert(uint16_t alertAddr, const std::vector<ValueType> &values);
    template <typename ValueType, size_t TypeSize = 8>
    void detailsAlert(const ValueType &value, size_t site, uint16_t dbNumber,
                      std::map<std::string, std::string> &mapCurrentAlert);
  signals:
    void signalIn(const uint32_t count);     // 进料信号 count是需要创建的个数 大于等于1
    void signalOut(const uint32_t count);    // 出料信号
    void signalQR(const uint32_t count);     // 扫码信号 针对回转式而言 此信号相当于原bottlemove信号
    void signalPrint(const uint32_t count);  // 打码信号
    void signalOCR(const uint32_t count);    // OCR信号
    void signalLocate(const uint32_t count); // 定位相机信号
    void signalCheck(const uint32_t count);  // 复核相机信号

    void signalState(const uint32_t status); // 状态改变信号
    void signalStep(const uint32_t steps);   // 设备步骤改变信号
    void triggerImage();

  protected:
    BasePLCDevice(const BasePLCDevice &) = delete;
    BasePLCDevice &operator=(const BasePLCDevice &) = delete;
    // 通用保护接口，无需子类化

    bool initAddress(const QString &path);
    bool initAlert(const QString &path);
    template <typename ValueType> void updateCaches(uint16_t address, const std::vector<ValueType> &candidates)
    {
        std::lock_guard lock(mtxCaches_);
        auto destination = caches_.begin() + (address - cacheAddress_);

        if constexpr (std::is_same_v<ValueType, uint8_t>)
        {
            // 如果 ValueType 是 uint8_t，则进行类型转换后拷贝
            std::transform(candidates.begin(), candidates.end(), destination,
                           [](uint8_t value) { return static_cast<uint16_t>(value); });
        }
        else if constexpr (std::is_same_v<ValueType, uint16_t>)
        {
            // 如果 ValueType 是 uint16_t，则直接拷贝
            std::copy(candidates.begin(), candidates.end(), destination);
        }
    }
    virtual void idCompare(const uint32_t &currentId, const DeviceUnion key);
    void stateCompare(const uint32_t &currentState, const DeviceUnion key);
    // 以下接口子类化
    virtual bool additionalInit() = 0; // 补充任务开始前的初始化行为
    virtual bool clearArguments() = 0; // 设置计数清零
    virtual bool beforeClose();        // 任务关闭前的行为
    virtual void parseFast() = 0;      // 解析快取寄存器
    virtual void parseLazy() = 0;      // 解析慢速寄存器
    // 数据成员
    std::unique_ptr<BaseClient> client_{nullptr};
    DeviceUnion devType;
    std::atomic_bool isTesting = false;
    std::atomic_bool bReady_ = false;

    std::vector<uint16_t> caches_; // 容器缓存
    uint16_t cacheAddress_ = 0;
    size_t cacheSize_ = 0;
    uint16_t dbNumber_ = 0;

    std::mutex mtxCaches_;
    std::unordered_map<DeviceUnion, uint32_t> mapDevDatas_;
    std::unordered_map<DeviceUnion, std::function<void(const uint32_t)>> mapDevSignals_;
    uint8_t heartbeat_ = 0;
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
    if (protocol_ == "snap7")
    {
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
                size_t low = i + 1;
                size_t high = i;
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
                size_t low = i + 1;
                size_t high = i;
                uint32_t value = retWords[low] | (retWords[high] << 16);
                values.push_back(value);
            }

            return values;
        }
        else if constexpr (std::is_same_v<ValueType, float>)
        {
            std::vector<uint32_t> retDInts = readArray<uint32_t>(addr, size);
            std::vector<float> values;
            for (const auto &temp : retDInts)
            {
                values.push_back(temp / 100.0f);
            }
            return values;
        }
    }
    else
    {
        if constexpr (std::is_same_v<ValueType, uint16_t>)
        {
            std::vector<uint16_t> retVec;

            // 计算起始索引和结束索引
            size_t startIndex = addr - cacheAddress_;
            size_t endIndex = std::min(startIndex + size, caches_.size());
            // 使用 std::copy_if 将数据从缓存中复制到返回向量中
            std::lock_guard lock(mtxCaches_);
            std::copy_if(caches_.begin() + startIndex, caches_.begin() + endIndex, std::back_inserter(retVec),
                         [](uint16_t value) { return true; }); // 如果你有其他过滤条件，可以在这里添加
            return retVec;
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
                uint32_t value = retWords[low] | (retWords[high] << 16);
                values.push_back(value);
            }
            return values;
        }
        else if constexpr (std::is_same_v<ValueType, float>)
        {
            std::vector<uint32_t> retDInts = readArray<uint32_t>(addr, size);
            std::vector<float> values;
            for (const auto &temp : retDInts)
            {
                values.push_back(temp / 100.0f);
            }
            return values;
        }
    }
    return {};
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

template <typename ValueType> void BasePLCDevice::parseAlert(uint16_t alertAddr, const std::vector<ValueType> &alerts)
{
    size_t bitLength = 0;
    uint16_t dbNumber = 0;
    if constexpr (std::is_same_v<ValueType, uint8_t>)
    {
        bitLength = 8;
        dbNumber = 2;
    }
    else if constexpr (std::is_same_v<ValueType, uint16_t>)
    {
        bitLength = 16;
    }
    std::map<std::string, std::string> mapCurrentAlert;
    for (size_t i = 0; i < alerts.size(); i++)
    {
        if (alerts[i] > 0)
        {
            if (bitLength == 8)
            {
                detailsAlert<uint8_t, 8>(alerts[i], i + alertAddr, dbNumber, mapCurrentAlert);
            }
            else if (bitLength == 16)
            {
                detailsAlert<uint16_t, 16>(alerts[i], i + alertAddr, dbNumber, mapCurrentAlert);
            }
        }
    }
    AlertWapper::updateRealtimeAlert(mapCurrentAlert, mapRecordAlert_);
}

template <typename ValueType, size_t TypeSize>
inline void BasePLCDevice::detailsAlert(const ValueType &value, size_t site, uint16_t dbNumber,
                                        std::map<std::string, std::string> &mapCurrentAlert)
{
    std::bitset<TypeSize> temp(value);

    for (size_t j = 0; j < TypeSize; j++)
    {
        if (temp.test(j))
        {
            // 计算 key
            const std::string key = fmt::format("{}_{}_{}", dbNumber, site, j);

            auto finder = mapAlertStore_.find(key);
            if (finder != mapAlertStore_.end())
            {
                // qDebug() << "parsAlertInfo key = " << key;
                mapCurrentAlert[key] = finder->second;
            }
        }
    }
}