#pragma once
#include "LineProduct.h"

LineProduct::LineProduct()
{
}

LineProduct::~LineProduct()
{
}

void LineProduct::signalQR(uint32_t pdNum)
{
    ++curBottleNum_;
    auto pd = std::make_shared<ProductItem>(curBottleNum_, pdType_, "", "");
    pd->QRSigTime = Utils::getCurrentTime(true);
    std::lock_guard lock(mtxQR_);
    lvQR_.push_back(pd);
}

void LineProduct::signalLocation()
{
}

void LineProduct::signalCheck()
{
}

std::shared_ptr<ProductItem> LineProduct::signalCoding()
{
    std::shared_ptr<ProductItem> item = nullptr;
    if (lvQR_.size() > 0)
    {
        std::lock_guard lock(mtxQR_);
        item = lvQR_.front();
        lvQR_.pop_front();
    }
    if (item)
    {
        item->codingSigTime = Utils::getCurrentTime(true);
        std::lock_guard lock(mtxCoding_);
        lvCoding_.push_back(item);
    }
    return item;
}

void LineProduct::signalOCR()
{
    std::shared_ptr<ProductItem> item = nullptr;
    if (lvCoding_.size() > 0)
    {
        item = lvCoding_.front();
        std::lock_guard lock(mtxCoding_);
        lvCoding_.pop_front();
    }
    if (item)
    {
        item->OCRSigTime = Utils::getCurrentTime(true);
        std::lock_guard lock(mtxOCR_);
        lvOCR_.push_back(item);
    }
}

void LineProduct::signalComplete()
{
    auto ptr = lvOCR_.front();
    // 插入数据库
    ptr->completeSigTime = Utils::getCurrentTime(true);
    ProductTimeWapper::insert(ptr);
    ProductDataWapper::insert(ptr);
    std::lock_guard lock(mtxOCR_);
    lvOCR_.pop_front();
}

void LineProduct::updateQRCode(const std::string &code)
{
    std::lock_guard lock(mtxQR_);
    for (auto ptr : lvQR_)
    {
        if (ptr->QRCodeTime.empty())
        {
            ptr->QRCode = code;
            ptr->QRCodeTime = Utils::getCurrentTime(true);
            return;
        }
    }
}

void LineProduct::updateLogistics(const std::string &code1, const std::string &code2)
{
    std::lock_guard lock(mtxQR_);
    for (auto ptr : lvQR_)
    {
        if (ptr->logisticsTime.empty())
        {
            ptr->logistics1 = code1;
            ptr->logistics1 = code2;
            ptr->logisticsTime = Utils::getCurrentTime(true);
            return;
        }
    }
}

uint32_t LineProduct::updateLocation(const cv::Mat &mat, const std::string &path)
{
    return 0;
}

std::shared_ptr<ProductItem> LineProduct::updateLocationResult(const uint32_t number, const std::string &value)
{
    return nullptr;
}

uint32_t LineProduct::updateCheck(const cv::Mat &mat, const std::string &path)
{
    return 0;
}

std::shared_ptr<ProductItem> LineProduct::updateCheckResult(const uint32_t number, const std::string &value)
{
    return nullptr;
}

uint32_t LineProduct::updateOCR(const cv::Mat &mat, const std::string &path)
{
    std::lock_guard lock(mtxOCR_);
    for (auto ptr : lvOCR_)
    {
        if (ptr->OCRImageTime.empty())
        {
            ptr->OCRImage = mat;
            ptr->OCRPath = path;
            ptr->OCRImageTime = Utils::getCurrentTime(true);
            return ptr->bottleNum_;
        }
    }
    return 0;
}

std::shared_ptr<ProductItem> LineProduct::updateOCRResult(const uint32_t number, const std::string &value)
{
    std::lock_guard lock(mtxOCR_);
    for (auto ptr : lvOCR_)
    {
        if (ptr->bottleNum_ == number)
        {
            ptr->OCRResultTime = Utils::getCurrentTime(true);
            ptr->OCRResult = value;
            std::string cp = ptr->logistics1 + ptr->logistics2;
            if (value == cp)
            {
                ptr->isComplete_ = true;
            }
            return ptr;
        }
    }
    return nullptr;
}

std::shared_ptr<ProductItem> LineProduct::getIndexObject(uint32_t index)
{
    return std::shared_ptr<ProductItem>();
}
