#pragma once
#include "Logger.h"
#include "NonCopyable.h"
#include "Utils.h"
#include <bitset>
#include <memory>

enum class TypeProduct
{
    TypeBase,
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

class Count
{
  public:
    Count() = default;
    ~Count() = default;
    virtual void zeroClear()
    {
        for (auto iter = countData.begin(); iter != countData.end(); iter++)
        {
            iter->second = 0;
        }
    };
    virtual void zeroClear(const std::string countItem)
    {
        auto item = countData.find(countItem);
        if (item != countData.end())
        {
            item->second = 0;
        }
    };
    virtual std::unordered_map<std::string, uint32_t>& dataRead()
    {
        return countData;
    };
    virtual void dataAdd(const std::string countItem)
    {
        auto item = countData.find(countItem);
        if (item != countData.end())
        {
            item->second++;
        }
    };

  protected:
    std::unordered_map<std::string, uint32_t> countData;
};

struct ProductItem
{
    ProductItem()
    {
    }
    ProductItem(uint32_t num, TypeProduct type, std::string batchNum, std::string formulaName)
        : bottleNum_(num), typePd_(type), batchNum_(batchNum), formulaName_(formulaName)
    {
    }
    // 公共数据
    TypeProduct typePd_{TypeProduct::TypeBase};
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
    std::string QRCodeSigTime;         // 二维码读码信号时间
    std::string QRCodeTime;            // 二维码读码返回时间
    std::string logisticsTime;         // 物流码返回时间
    std::string LocationSigTime;       // 定位信号时间
    std::string LocationImageTime;     // 定位图像获取时间
    std::string LocationResultTime;    // 定位结果获取时间
    std::string CheckSigTime;          // 定位复核信号时间
    std::string CheckImageTime;        // 定位复核图像获取时间
    std::string CheckResultTime;       // 定位复核结果获取时间
    std::string codingSigTime;         // 物流码打码信号时间
    std::string OCRSigTime;            // OCR信号时间
    std::string OCRImageTime;          // OCR图像获取时间
    std::string OCRResultTime;         // OCR结果获取时间
    std::string removeSigTime;         // 流程完成时间（踢出信号时间）
    std::string issuedRotateTime;      // 回转式下发旋转时间
    std::string issuedLocateCheckTime; // 回转式下发定位复合时间
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
    virtual void createProduct(uint32_t pdNum, const std::string &batchNum = "", const std::string &formulaName = "")
    {
        std::lock_guard lock(mtxProduct_);
        auto pd = std::make_shared<ProductItem>(pdNum, pdType_, batchNum, formulaName);
        qProduct_.push_front(pd);
        if (qProduct_.size() > 17)
        {
            qProduct_.pop_back();
        }
        effectiveCursor(pdNum);
    }
    virtual std::shared_ptr<ProductItem> deleteProduct()
    {
        std::lock_guard lock(mtxProduct_);
        auto ptr = qProduct_.back();
        qProduct_.pop_back();
        return ptr;
    }
    virtual void signalQR()
    {
        std::lock_guard lock(mtxProduct_);
        qProduct_[cursorQRSignal_]->QRCodeSigTime = Utils::getCurrentTime(true);
        moveCursor(cursorQRSignal_);
    }
    virtual void signalLocation()
    {
        std::lock_guard lock(mtxProduct_);
        qProduct_[cursorLocationSignal_]->LocationSigTime = Utils::getCurrentTime(true);
        moveCursor(cursorLocationSignal_);
    }
    virtual void signalCheck()
    {
        std::lock_guard lock(mtxProduct_);
        qProduct_[cursorCheck_]->CheckSigTime = Utils::getCurrentTime(true);
        moveCursor(cursorCheck_);
    }
    virtual std::shared_ptr<ProductItem> signalCoding()
    {
        std::lock_guard lock(mtxProduct_);
        auto ptr = qProduct_[cursorCodingSignal_];
        ptr->codingSigTime = Utils::getCurrentTime(true);
        moveCursor(cursorCheck_);
        return ptr;
    }
    virtual void signalOCR()
    {
        std::lock_guard lock(mtxProduct_);
        qProduct_[cursorOCRSignal_]->OCRSigTime = Utils::getCurrentTime(true);
        moveCursor(cursorOCRSignal_);
    }
    virtual void signalRemove()
    {
        std::lock_guard lock(mtxProduct_);
        qProduct_[cursorRemoveSignal_]->removeSigTime = Utils::getCurrentTime(true);
        moveCursor(cursorRemoveSignal_);
    }

    virtual uint32_t updateQRCode(const std::string &code)
    {
        std::lock_guard lock(mtxProduct_);
        auto ptr = qProduct_[cursorQRCode_];
        ptr->QRCode = code;
        ptr->QRCode = Utils::getCurrentTime(true);
        moveCursor(cursorQRCode_);
        return ptr->bottleNum_;
    }

    virtual void updateLogistics(const uint32_t number, const std::string &code1, const std::string &code2)
    {
        std::lock_guard lock(mtxProduct_);
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
        std::lock_guard lock(mtxProduct_);
        auto ptr = qProduct_[cursorLocation_];
        ptr->LocationImage = mat;
        ptr->LocationPath = path;
        ptr->LocationImageTime = Utils::getCurrentTime(true);
        moveCursor(cursorLocation_);
        return ptr->bottleNum_;
    }

    virtual std::shared_ptr<ProductItem> updateLocationResult(const uint32_t number, const std::string &value)
    {
        std::lock_guard lock(mtxProduct_);
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
        std::lock_guard lock(mtxProduct_);
        auto ptr = qProduct_[cursorCheck_];
        ptr->CheckImage = mat;
        ptr->CheckPath = path;
        ptr->CheckImageTime = Utils::getCurrentTime(true);
        moveCursor(cursorCheck_);
        return ptr->bottleNum_;
    }

    virtual std::shared_ptr<ProductItem> updateCheckResult(const uint32_t number, const std::string &value)
    {
        std::lock_guard lock(mtxProduct_);
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
        std::lock_guard lock(mtxProduct_);
        auto ptr = qProduct_[cursorOCR_];
        ptr->OCRImage = mat;
        ptr->OCRPath = path;
        ptr->OCRImageTime = Utils::getCurrentTime(true);
        moveCursor(cursorOCR_);
        return ptr->bottleNum_;
    }

    virtual std::shared_ptr<ProductItem> updateOCRResult(const uint32_t number, const std::string &value)
    {
        std::lock_guard lock(mtxProduct_);
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
        std::lock_guard lock(mtxProduct_);
        if (index < qProduct_.size())
        {
            auto ptr = qProduct_[index];
            if (ptr->bottleNum_ > 0)
            {
                return ptr;
            }
        }
        return nullptr;
    }

  protected:
    void effectiveCursor(const uint32_t &bottleNumber)
    {
        if (cursorQRSignal_ > -1 || bottleNumber > 0)
        {
            ++cursorQRSignal_;
        }
        if (cursorLocationSignal_ > -1 || bottleNumber > 0)
        {
            ++cursorLocationSignal_;
        }
        if (cursorCheckSignal_ > -1 || bottleNumber > 0)
        {
            ++cursorCheckSignal_;
        }
        if (cursorCodingSignal_ > -1 || bottleNumber > 0)
        {
            ++cursorCodingSignal_;
        }
        if (cursorOCRSignal_ > -1 || bottleNumber > 0)
        {
            ++cursorOCRSignal_;
        }
        if (cursorRemoveSignal_ > -1 || bottleNumber > 0)
        {
            ++cursorRemoveSignal_;
        }
        if (cursorQRCode_ > -1 || bottleNumber > 0)
        {
            ++cursorQRCode_;
        }
        if (cursorLocation_ > -1 || bottleNumber > 0)
        {
            ++cursorLocation_;
        }
        if (cursorCheck_ > -1 || bottleNumber > 0)
        {
            ++cursorCheck_;
        }
        if (cursorOCR_ > -1 || bottleNumber > 0)
        {
            ++cursorOCR_;
        }
    }
    void moveCursor(int16_t &cursor)
    {
        while (true)
        {
            --cursor;
            if (cursor <= -1 || qProduct_[cursor]->bottleNum_ > 0)
            {
                break;
            }
        }
    }
    TypeProduct pdType_{TypeProduct::TypeBase};
    std::deque<std::shared_ptr<ProductItem>> qProduct_;
    std::mutex mtxProduct_;
    int16_t cursorQRSignal_ = -1;
    int16_t cursorLocationSignal_ = -1;
    int16_t cursorCheckSignal_ = -1;
    int16_t cursorCodingSignal_ = -1;
    int16_t cursorOCRSignal_ = -1;
    int16_t cursorRemoveSignal_ = -1;
    int16_t cursorQRCode_ = -1;
    int16_t cursorLocation_ = -1;
    int16_t cursorCheck_ = -1;
    int16_t cursorOCR_ = -1;
};