#pragma once
#include "Logger.h"
#include "NonCopyable.h"
#include "Utils.h"
#include <bitset>
#include <memory>

enum class TypeProduct
{
    TypeCircle,
    TypeLine,
    TypeCap
};

struct OcrRes
{
    OcrRes(std::string result, uint16_t lefttopx, uint16_t lefttopy, uint16_t leftbottomx, uint16_t leftbottomy,
           uint16_t righttopx, uint16_t righttopy, uint16_t rightbottomx, uint16_t rightbottomy)
        : result(result), lefttopx(lefttopx), lefttopy(lefttopy), leftbottomx(leftbottomx), leftbottomy(leftbottomy),
          righttopx(righttopx), righttopy(righttopy), rightbottomx(rightbottomx), rightbottomy(rightbottomy)
    {
    }
    std::string result;
    uint16_t lefttopx;
    uint16_t lefttopy;
    uint16_t leftbottomx;
    uint16_t leftbottomy;
    uint16_t righttopx;
    uint16_t righttopy;
    uint16_t rightbottomx;
    uint16_t rightbottomy;
};

struct ProductData
{
    ProductData() = default;
    uint32_t countAll = 0;
    uint32_t countPass = 0;
    uint32_t countWaste = 0;
    uint32_t countLocateWaste = 0;
    uint32_t countCodeWaste = 0;
    uint32_t countPauseWaste = 0;
};

struct ProductItem
{
    ProductItem(uint32_t num, TypeProduct type, std::string batchNum, std::string formulaName)
        : bottleNum_(num), typePd_(type), batchNum_(batchNum), formulaName_(formulaName)
    {
    }
    // 公共数据
    TypeProduct typePd_;
    uint32_t bottleNum_ = 0;  // 应用层维护瓶编号，自增
    std::string batchNum_;    // 批号
    std::string formulaName_; // 配方名
    bool isRemove_ = false;   // 是否需要踢瓶
    // 数据表
    std::string QRCode;         // 二维码读码结果
    std::string logistics1;     // 物流码真实值1
    std::string logistics2;     // 物流码真实值2
    std::string LocationPath;   // 定位图像路径
    std::string CheckPath;      // 定位复核图像路径
    std::string OCRPath;        // OCR图像路径
    cv::Mat LocationImage;      // 定位图像
    cv::Mat CheckImage;         // 定位复核图像
    cv::Mat OCRImage;           // OCR图像
    std::string LocationResult; // 定位结果
    std::string CheckResult;    // 定位复核结果
    std::string OCRResult;      // 物流码结果

    // 时间点表
    std::string QRCodeTime;         // 二维码读码返回时间
    std::string logisticsTime;      // 物流码返回时间
    std::string LocationSigTime;    // 定位信号时间
    std::string LocationImageTime;  // 定位图像获取时间
    std::string LocationResultTime; // 定位结果获取时间
    std::string CheckSigTime;       // 定位复核信号时间
    std::string CheckImageTime;     // 定位复核图像获取时间
    std::string CheckResultTime;    // 定位复核结果获取时间
    std::string codingSigTime;      // 物流码打码信号时间
    std::string OCRSigTime;         // OCR信号时间
    std::string OCRImageTime;       // OCR图像获取时间
    std::string OCRResultTime;      // OCR结果获取时间
    std::string completeSigTime;    // 流程完成时间（踢出信号时间）
};

class BaseProduct : public AppFrame::NonCopyable
{
  public:
    explicit BaseProduct() = default;
    virtual ~BaseProduct() = default;
    const TypeProduct &getType()
    {
        return pdType_;
    }
    virtual void signalQR(uint32_t pdNum = 0)
    {
    }
    virtual void signalLocation()
    {
    }
    virtual void signalCheck()
    {
    }
    virtual std::shared_ptr<ProductItem> signalCoding()
    {
        return nullptr;
    }
    virtual void signalOCR()
    {
    }
    virtual void signalComplete()
    {
    }

    virtual void updateQRCode(const std::string &code)
    {
        ++curBottleNum_;
        auto pd = std::make_shared<ProductItem>(curBottleNum_, pdType_, "", "");
        qProduct_.push_front(pd);

        pd->QRCode = code;
        pd->QRCodeTime = Utils::getCurrentTime(true);
        // 测试代码！！！测试完成删除
        std::string currentTime = Utils::getCurrentTime(false).substr(11, 8);
        currentTime.erase(std::remove(currentTime.begin(), currentTime.end(), ':'), currentTime.end());
        currentTime += "abcabc";
        pd->logistics1 = "123abcabc123";
        pd->logistics2 = currentTime;
        pd->logisticsTime = Utils::getCurrentTime(true);
    }

    virtual void updateLogistics(const uint32_t number, const std::string &code1, const std::string &code2)
    {
        for (auto ptr = qProduct_.rbegin(); ptr != qProduct_.rend(); ++ptr)
        {
            if ((*ptr)->bottleNum_ == number)
            {
                (*ptr)->logistics1 = code1;
                (*ptr)->logistics2 = code2;
                (*ptr)->logisticsTime = Utils::getCurrentTime(true);
                return;
            }
        }
    }

    virtual uint32_t updateLocation(const cv::Mat &mat, const std::string &path)
    {
        for (auto ptr = qProduct_.rbegin(); ptr != qProduct_.rend(); ++ptr)
        {
            if ((*ptr)->bottleNum_ > 0 && (*ptr)->LocationImageTime.empty())
            {
                (*ptr)->LocationImage = mat;
                (*ptr)->LocationPath = path;
                (*ptr)->LocationImageTime = Utils::getCurrentTime(true);
                return (*ptr)->bottleNum_;
            }
        }
        return 0;
    }

    virtual std::shared_ptr<ProductItem> updateLocationResult(const uint32_t number, const std::string &value)
    {
        for (auto ptr = qProduct_.rbegin(); ptr != qProduct_.rend(); ++ptr)
        {
            if ((*ptr)->bottleNum_ == number)
            {
                (*ptr)->LocationResult = value;
                (*ptr)->LocationResultTime = Utils::getCurrentTime(true);
                return *ptr;
            }
        }
        return nullptr; // 或者根据需要返回适当的值
    }

    virtual uint32_t updateCheck(const cv::Mat &mat, const std::string &path)
    {
        for (auto ptr = qProduct_.rbegin(); ptr != qProduct_.rend(); ++ptr)
        {
            if ((*ptr)->bottleNum_ > 0 && (*ptr)->CheckImageTime.empty())
            {
                (*ptr)->CheckImage = mat;
                (*ptr)->CheckPath = path;
                (*ptr)->CheckImageTime = Utils::getCurrentTime(true);
                return (*ptr)->bottleNum_;
            }
        }
        return 0;
    }

    virtual std::shared_ptr<ProductItem> updateCheckResult(const uint32_t number, const std::string &value)
    {
        for (auto ptr = qProduct_.rbegin(); ptr != qProduct_.rend(); ++ptr)
        {
            if ((*ptr)->bottleNum_ == number)
            {
                (*ptr)->CheckResult = value;
                (*ptr)->CheckResultTime = Utils::getCurrentTime(true);
                return *ptr;
            }
        }
        return nullptr; // 或者根据需要返回适当的值
    }

    virtual uint32_t updateOCR(const cv::Mat &mat, const std::string &path)
    {
        for (auto ptr = qProduct_.rbegin(); ptr != qProduct_.rend(); ++ptr)
        {
            if ((*ptr)->bottleNum_ > 0 && (*ptr)->OCRImageTime.empty())
            {
                (*ptr)->OCRImage = mat;
                (*ptr)->OCRPath = path;
                (*ptr)->OCRImageTime = Utils::getCurrentTime(true);
                return (*ptr)->bottleNum_;
            }
        }
        return 0;
    }

    virtual std::shared_ptr<ProductItem> updateOCRResult(const uint32_t number, const std::string &value)
    {
        for (auto ptr = qProduct_.rbegin(); ptr != qProduct_.rend(); ++ptr)
        {
            if ((*ptr)->bottleNum_ == number)
            {
                (*ptr)->OCRResult = value;
                (*ptr)->OCRResultTime = Utils::getCurrentTime(true);
                return *ptr;
            }
        }
        return nullptr; // 或者根据需要返回适当的值
    }

    virtual std::shared_ptr<ProductItem> getIndexObject(uint32_t index)
    {
        if (index >= qProduct_.size())
        {
            return nullptr;
        }
        return qProduct_[index];
    }

  protected:
    uint32_t curBottleNum_{0};
    TypeProduct pdType_{TypeProduct::TypeCircle};
    std::deque<std::shared_ptr<ProductItem>> qProduct_;
};