#ifndef BASEPRODUCT_H
#define BASEPRODUCT_H

#include "Logger.h"

#include "NonCopyable.h"
#include <QCoreApplication>
#include <QEventLoop>
#include <QObject>
#include <QTimer>
#include <bitset>
#include <cstdint>
#include <deque>
#include <memory>
#include <opencv2/core/mat.hpp>
#include <string>
#include <unordered_map>
#include <yaml-cpp/yaml.h>

enum class TypeProduct
{
    TypeBase = 0,
    TypeCircle = 1,
    TypeLine = 2,
    TypeCap = 3,
    TypeImpurity = 4,
    TYpeBoxApp = 5
};

enum class TypeQueueToPLC
{
    Print,
    Rotating,
    Remove,
    ReRotating
};

enum class SubType
{
    ClockWise,
    AntiClockWise
};

enum class BottleShape
{
    Round,
    Square
};

struct ProductImgaeData
{
    std::string path_;
    cv::Mat image_;
    ProductImgaeData()
    {
    }
    ProductImgaeData(const std::string &str, const cv::Mat &mat) : path_(str), image_(mat)
    {
    }
};
// 定义值的变体类型
using ValueVariant = std::variant<uint8_t, std::string, ProductImgaeData>;

struct ProductItemKey
{
    inline static std::string type_pd{"type_pd"};
    inline static std::string batch_num{"batch_num"};
    inline static std::string formula_name{"formula_name"};
    inline static std::string bottle_num{"bottle_num"};
    inline static std::string is_remove{"is_remove"};

    inline static std::string qr_code{"qr_code"};                     // 二维码读码结果
    inline static std::string logistics{"logistics"};                 // 物流码
    inline static std::string location_path{"location_path"};         // 定位图像路径
    inline static std::string check_path{"check_path"};               // 定位复核图像路径
    inline static std::string ocr_path{"ocr_path"};                   // OCR图像路径
    inline static std::string location_result{"location_result"};     // 定位结果
    inline static std::string check_result{"check_result"};           // 定位复核结果
    inline static std::string rerotate_result{"rerotate_result"};     // 二次旋转结果
    inline static std::string ocr_result{"ocr_result"};               // 物流码结果
    inline static std::string is_ocr_equal_code{"is_ocr_equal_code"}; // 物流码与OCR对比结果
    inline static std::string rotating_values{"rotating_values"};     // 下发的16组旋转值
    inline static std::string coding_values{"coding_values"};         // 下发的16位打码信号
    inline static std::string remove_values{"remove_values"};         // 下发的32位踢瓶信号
    inline static std::string rerotating_values{"rerotating_values"}; // 二次旋转角度
};

class ProductItem
{
  public:
    ProductItem()
    {
    }
    ProductItem(uint32_t num) : bottleNum_(num)
    {
    }
    uint32_t bottleNumber() const
    {
        return bottleNum_;
    }
    void setRemoveTrue()
    {
        isRemove_ = true;
    }
    bool isRemove() const
    {
        return isRemove_;
    }
    void setPrintFalse()
    {
        isPrint_ = false;
    }
    bool isPrint() const
    {
        return isPrint_;
    }
    void setValue(const std::string &key, ValueVariant value)
    {
        mapProcessData_[key] = value;
    }

    template <typename NeedType> NeedType getValue(const std::string &key)
    {
        auto iter = mapProcessData_.find(key);
        if (iter == mapProcessData_.end())
        {
            LogInfo("key: {} not exist", key);
            return NeedType{};
        }
        else if (std::holds_alternative<NeedType>(iter->second) == false)
        {
            return NeedType{};
        }
        else
        {
            LogInfo("key: {} normal accept", key);
            return std::get<NeedType>(iter->second);
        }
    }

  private:
    uint32_t bottleNum_ = 0; // 小瓶编号
    bool isRemove_ = false;  // 是否需要踢瓶 默认剔除
    bool isPrint_ = true;    // 是否需要打印 默认打印
    // 可扩展数据表
    std::unordered_map<std::string, ValueVariant> mapProcessData_;
};

template <typename ReturnType> class QueueGetter;

template <> class QueueGetter<std::vector<uint16_t>>
{
  public:
    static std::vector<uint16_t> get(const std::vector<uint32_t> &queue)
    {
        std::vector<uint16_t> ret;
        for (const auto &temp : queue)
        {
            ret.push_back(static_cast<uint16_t>(temp));
            ret.push_back(static_cast<uint16_t>(temp >> 16));
        }
        return ret;
    }

    static std::vector<uint16_t> get(const std::vector<uint8_t> &queue)
    {
        std::vector<uint16_t> ret;
        std::bitset<16> bits;

        for (size_t i = 0; i < queue.size(); ++i)
        {
            bits[i % 16] = queue[i];

            if ((i + 1) % 16 == 0 || i == queue.size() - 1)
            {
                ret.push_back(bits.to_ulong());
                bits.reset();
            }
        }
        return ret;
    }
};

template <> class QueueGetter<std::vector<uint8_t>>
{
  public:
    static std::vector<uint8_t> get(const std::vector<uint32_t> &queue)
    {
        return {};
    }
    static std::vector<uint8_t> get(const std::vector<uint8_t> &queue)
    {
        std::vector<uint8_t> ret;
        std::bitset<8> bits;
        for (size_t i = 0; i < queue.size(); ++i)
        {
            bits[i % 8] = queue[i];

            if ((i + 1) % 8 == 0 || i == queue.size() - 1)
            {
                ret.push_back(bits.to_ulong());
                bits.reset();
            }
        }
        return ret;
    }
};

class BaseProduct : public QObject, public AppFrame::NonCopyable
{
    Q_OBJECT

  public:
    explicit BaseProduct(QObject *parent = nullptr);
    virtual ~BaseProduct() = default;
    const TypeProduct &getType();
    virtual void init(const YAML::Node &config = YAML::Node());
    virtual void clearQueue(); // plc队列清零
    void clearProductId();     // 产品id清零
    void setAllRemove();

    // 方瓶、圆瓶、回转方向、偏转角度设置
    SubType getSubType() const;
    BottleShape getBottleShape() const;
    void setBottleShape(BottleShape shape);
    int getOffset() const;

    virtual void createProduct(uint16_t count);
    virtual std::list<std::shared_ptr<ProductItem>> deleteProduct(uint16_t count);
    virtual std::shared_ptr<ProductItem> updateValue(const uint32_t productId, const std::string &key,
                                                     const ValueVariant &value);
    template <typename ReturnType>
    std::vector<ReturnType> getQueueToPLC(TypeQueueToPLC type, const ValueVariant &stored,
                                          std::shared_ptr<ProductItem> product); // 获取下发PLC队列
  protected:
    void judgment(const std::string &key, const ValueVariant &value, const std::shared_ptr<ProductItem> &product);
    std::string storeRotatingQueue(const std::string &);   // 存储旋转下发队列
    std::string storePrintQueue(uint8_t);                  // 存储打印队列
    std::string storeRemoveQueue(uint8_t);                 // 存储剔除队列
    std::string storeReRotatingQueue(const std::string &); // 存储二次旋转下发队列

    TypeProduct pdType_{TypeProduct::TypeBase};
    std::string batchNum_;                         // 批号
    std::string formulaName_;                      // 配方名
    SubType subType_;                              // 机器的旋转方向
    BottleShape bottleShape_ = BottleShape::Round; // 瓶型
    int roundOffset_;                              // 圆瓶旋转偏移
    int squareOffset_;                             // 方瓶旋转偏移
    std::deque<std::shared_ptr<ProductItem>> qProduct_;
    std::mutex mtxProduct_;

    uint32_t productId = 0;

    uint32_t rotatingQueueLength_ = 16;     // 旋转队列长度
    uint32_t rotatingIndex_ = 0;            // 旋转队列偏移
    std::vector<uint32_t> rotatingQueue_;   // 旋转数据队列
    uint32_t printQueueLength_ = 16;        // 打印队列长度
    uint32_t printIndex_ = 0;               // 打印队列偏移
    std::vector<uint8_t> printQueue_;       // 打印数据队列
    uint32_t removeQueueLength_ = 32;       // 踢出队列长度
    uint32_t removeIndex_ = 0;              // 踢出队列偏移
    std::vector<uint8_t> removeQueue_;      // 剔除数据队列
    std::vector<uint32_t> reRotatingQueue_; // 第二次旋转队列
    uint32_t reRotatingQueueLength_ = 16;   // 第二次旋转队列长度
    uint32_t reRotatingIndex_ = 0;          // 第二次旋转队列偏移
};

template <typename ReturnType>
inline std::vector<ReturnType> BaseProduct::getQueueToPLC(TypeQueueToPLC type, const ValueVariant &stored,
                                                          std::shared_ptr<ProductItem> product)
{
    std::lock_guard lock(mtxProduct_);
    std::vector<ReturnType> ret;
    switch (type)
    {
    case TypeQueueToPLC::Rotating: {
        std::string desQueue = storeRotatingQueue(std::get<std::string>(stored));
        ret = QueueGetter<std::vector<ReturnType>>::get(rotatingQueue_);
        LogInfo("rotating queue: {}", desQueue);
        product->setValue(ProductItemKey::rotating_values, desQueue);
        break;
    }
    case TypeQueueToPLC::Print: {
        std::string desQueue = storePrintQueue(std::get<uint8_t>(stored));
        ret = QueueGetter<std::vector<ReturnType>>::get(printQueue_);
        LogInfo("print queue: {}", desQueue);
        product->setValue(ProductItemKey::coding_values, desQueue);
        break;
    }
    case TypeQueueToPLC::Remove: {
        std::string desQueue = storeRemoveQueue(std::get<uint8_t>(stored));
        ret = QueueGetter<std::vector<ReturnType>>::get(removeQueue_);
        LogInfo("remove queue: {}", desQueue);
        product->setValue(ProductItemKey::remove_values, desQueue);
        break;
    }
    case TypeQueueToPLC::ReRotating: {
        std::string desQueue = storeReRotatingQueue(std::get<std::string>(stored));
        ret = QueueGetter<std::vector<ReturnType>>::get(reRotatingQueue_);
        LogInfo("reRotating queue: {}", desQueue);
        product->setValue(ProductItemKey::rerotating_values, desQueue);
        break;
    }
    default:
        break;
    }
    return ret;
}

#endif // BASEPRODUCT_H