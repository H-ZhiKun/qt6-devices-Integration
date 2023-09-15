#pragma once
#include "Logger.h"
#include "Utils.h"
#include <bitset>
#include <memory>
#include <mutex>
struct LineItem
{
    LineItem(uint8_t num) : bottleNum(num)
    {
    }
    uint8_t bottleNum = 0;       // 应用层维护瓶编号，自增
    std::string QRSigTime;       // 二维码读码信号时间
    std::string QRCode;          // 二维码读码结果
    std::string QRCodeTime;      // 二维码读码返回时间
    std::string logisticsDes;    // 物流码描述
    std::string logistics1;      // 物流码真实值1
    std::string logistics2;      // 物流码真实值2
    std::string logisticsTime;   // 物流码返回时间
    std::string CodingSigTime;   // 物流码打码信号时间
    std::string OCRImagePath;    // 打码复核图像路径
    std::string OCRImageTime;    // 打码复核图像获取时间
    std::string OCRALGOTime;     // 打码复核算法返回时间
    cv::Mat OCRImage;            // 打码复核图像
    std::string OCRResult;       // 物流码打码结果
    bool isComplete = false;     // 流程完成状态
    std::string completeSigTime; // 流程完成时间（踢出信号发送时间）
};

class LineProduct
{
  public:
    explicit LineProduct() = default;
    ~LineProduct() = default;
    void newProduct()
    {
        ++curBottleNum_;
        auto ptr = std::make_shared<LineItem>(curBottleNum_);
        ptr->QRSigTime = Utils::getCurrentTime(true);
        std::lock_guard lock(mtxProduct_);
        lvProduct_.push_front(ptr);
    }
    uint32_t completeProduct()
    {
        std::shared_ptr<LineItem> ptr = nullptr;
        std::lock_guard lock(mtxProduct_);
        ptr = lvProduct_.back();
        ptr->completeSigTime = Utils::getCurrentTime(true);
        lvProduct_.pop_back();
        if (ptr->isComplete)
            completeBitValue = (completeBitValue >> 1);
        else
            completeBitValue = (completeBitValue >> 1) | (1 << 31);
        return completeBitValue;
    }
    uint8_t updateQRCode(const std::string &code)
    {
        if (lvProduct_.size() == 0)
            return 0;
        std::lock_guard lock(mtxProduct_);
        auto ptr = lvProduct_.front();
        ptr->QRCode = code;
        ptr->QRCodeTime = Utils::getCurrentTime(true);
        return ptr->bottleNum;
    }
    void updateLogistics(const uint32_t number, const std::string &logisticsDes, const std::string &code1,
                         const std::string &code2)
    {
        std::lock_guard lock(mtxProduct_);
        for (auto iter = lvProduct_.begin(); iter != lvProduct_.end(); ++iter)
        {
            if ((*iter)->bottleNum == number)
            {
                (*iter)->logistics1 = code1;
                (*iter)->logistics2 = code2;
                (*iter)->logisticsDes = logisticsDes;
                (*iter)->logisticsTime = Utils::getCurrentTime(true);
                break;
            }
        }
    }
    void updateCodingSigTime(std::string &code1, std::string code2)
    {

        std::lock_guard lock(mtxProduct_);
        for (auto iter = lvProduct_.rbegin(); iter != lvProduct_.rend(); ++iter)
        {
            if ((*iter)->CodingSigTime.empty())
            {
                (*iter)->CodingSigTime = Utils::getCurrentTime(true);
                code1 = (*iter)->logistics1;
                code2 = (*iter)->logistics2;
                break;
            }
        }
    }
    uint8_t updateOCR(const cv::Mat &mat, const std::string &path)
    {
        if (lvProduct_.size() == 0)
            return;
        uint8_t num = 0;
        std::lock_guard lock(mtxProduct_);
        for (auto iter = lvProduct_.begin(); iter != lvProduct_.end(); ++iter)
        {
            if ((*iter)->OCRImagePath.empty())
            {
                (*iter)->OCRImagePath = path;
                (*iter)->OCRImage = mat;
                (*iter)->OCRImageTime = Utils::getCurrentTime(true);
                num = (*iter)->bottleNum;
                break;
            }
        }
        return num;
    }
    void updateOCRResult(const uint32_t number, const std::string &value)
    {
        if (lvProduct_.size() == 0)
            return;
        std::lock_guard lock(mtxProduct_);
        for (auto iter = lvProduct_.begin(); iter != lvProduct_.end(); ++iter)
        {
            if ((*iter)->bottleNum == number)
            {
                (*iter)->OCRResult = value;
                (*iter)->OCRALGOTime = Utils::getCurrentTime(true);
                break;
            }
        }
    }

  private:
    std::mutex mtxProduct_;
    std::deque<std::shared_ptr<LineItem>> lvProduct_;
    uint8_t curBottleNum_{0};
    uint32_t completeBitValue = 0;
};