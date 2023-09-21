#pragma once
#include "Logger.h"
#include "NonCopyable.h"
#include "PLCDevice.h"
#include "Utils.h"
#include <bitset>
#include <memory>
#include <mutex>

enum class TypeProduct
{
    TypeCircle,
    TypeLine,
    TypeCap
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
    bool isComplete_ = false; // 流程完成状态
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
    std::string QRSigTime;          // 二维码读码信号时间
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
    virtual void complete() = 0;
    virtual void signalQR(uint32_t pdNum = 0) = 0;
    virtual void signalLocation() = 0;
    virtual void signalCheck() = 0;
    virtual std::shared_ptr<ProductItem> signalCoding() = 0;
    virtual void signalOCR() = 0;
    virtual void signalComplete() = 0;

    virtual void updateQRCode(const std::string &code) = 0;
    virtual void updateLogistics(const std::string &code1, const std::string &code2) = 0;
    virtual uint32_t updateLocation(const cv::Mat &mat, const std::string &path) = 0;
    virtual std::shared_ptr<ProductItem> updateLocationResult(const uint32_t number, const std::string &value) = 0;
    virtual uint32_t updateCheck(const cv::Mat &mat, const std::string &path) = 0;
    virtual std::shared_ptr<ProductItem> updateCheckResult(const uint32_t number, const std::string &value) = 0;
    virtual uint32_t updateOCR(const cv::Mat &mat, const std::string &path) = 0;
    virtual std::shared_ptr<ProductItem> updateOCRResult(const uint32_t number, const std::string &value) = 0;
    virtual std::shared_ptr<ProductItem> getIndexObject(uint32_t index) = 0;
};