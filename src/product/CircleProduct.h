#pragma once
#include "Logger.h"
#include "Utils.h"
#include <deque>
#include <memory>
#include <mutex>

struct ProductItem
{
    ProductItem(uint32_t number = 0) : numBottom(number)
    {
    }
    std::string batchNum;    // 批号
    std::string formulaName; // 配方名

    uint32_t numBottom = 0;
    std::string QRCode;       // 二维码读码结果
    std::string logisticsDes; // 物流码描述
    std::string logistics1;   // 物流码真实值1
    std::string logistics2;   // 物流码真实值2

    std::string locateImagePath;      // 定位图像路径
    std::string locateCheckImagePath; // 定位复核图像路径
    std::string codeCheckImagePath;   // 打码复核图像路径
    cv::Mat locateImage;              // 定位图像
    cv::Mat locateCheckImage;         // 定位复核图像
    cv::Mat codeCheckImage;           // 打码复核图像
    std::string locateResult;
    std::string locateCheckResult;
    std::string OCRResult; // 物流码预测值
    bool isComplete = false;
};

class CircleProduct
{
  public:
    explicit CircleProduct();
    ~CircleProduct();
    void newProduct(uint32_t number);
    void completeProduct();
    uint16_t updateQRCode(const std::string &code);
    void updateLogistics(const uint32_t number, const std::string &logisticsDes, const std::string &code1,
                         const std::string &code2);
    void updateLocate(const cv::Mat &mat, const std::string &path);
    void updateLocateResult(const uint32_t number, const std::string &value);
    void updateLocateCheck(const cv::Mat &mat, const std::string &path);
    void updateLocateCheckResult(const uint32_t number, const std::string &value);
    void updateOCR(const cv::Mat &mat, const std::string &path);
    void updateOCRResult(const uint32_t number, const std::string &value);
    const std::deque<ProductItem *> &getCircleProduct();
    const ProductItem *getNumber(const uint32_t number);

  private:
    std::mutex mtxProduct_;
    std::deque<ProductItem *> lvProduct_;
    uint16_t OffsetQRCode = 0;
    uint16_t OffsetLocate = 2;
    uint16_t OffsetLocateCheck = 3;
    uint16_t OffsetPrintCode = 4;
    uint16_t OffsetOCR = 5;
    uint16_t OffsetNG = 6;
};