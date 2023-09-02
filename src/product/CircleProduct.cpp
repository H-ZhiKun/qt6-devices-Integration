#include "CircleProduct.h"

CircleProduct::CircleProduct()
{
}

CircleProduct::~CircleProduct()
{
}

void CircleProduct::newProduct(uint32_t number)
{
    if (number > 0)
    {
        auto ptr = new ProductItem(number);
        std::lock_guard lock(mtxProduct_);
        lvProduct_.emplace_front(ptr);
    }
    else
    {
        std::lock_guard lock(mtxProduct_);
        lvProduct_.emplace_front(nullptr);
    }
}

void CircleProduct::completeProduct()
{
    std::lock_guard lock(mtxProduct_);
    const auto ptr = lvProduct_.back();
    LogInfo("product process:\r\ncomplete:\r\nnumber={},complete={}.", ptr->numBottom, ptr->isComplete);
    delete ptr;
    lvProduct_.pop_back();
}

uint16_t CircleProduct::updateQRCode(const std::string &code)
{
    auto ptr = lvProduct_[OffsetQRCode];
    if (ptr == nullptr)
        return 0;
    std::lock_guard lock(mtxProduct_);
    ptr->QRCode = code;
    LogInfo("product process:\r\nupdate QRCode:\r\ncode={}.", code);
    return OffsetQRCode;
}

void CircleProduct::updateLogistics(const uint32_t number, const std::string &logisticsDes, const std::string &code1,
                                    const std::string &code2)
{
    LogInfo("product process:\r\nupdate logistics:\r\nnumber={}, code1={}, code2={}.", number, code1, code2);
    std::lock_guard lock(mtxProduct_);
    for (const auto &ptr : lvProduct_)
    {
        if (ptr && ptr->numBottom == number)
        {
            ptr->logistics1 = code1;
            ptr->logistics2 = code2;
            ptr->logisticsDes = logisticsDes;
        }
    }
}

void CircleProduct::updateLocate(const cv::Mat &mat, const std::string &path)
{
    auto ptr = lvProduct_[OffsetLocate];
    if (ptr == nullptr)
        return;
    std::lock_guard lock(mtxProduct_);
    ptr->locateImage = mat;
    ptr->locateImagePath = path;
    LogInfo("product process:\r\nupdate locate:\r\nnumber={}, path={}.", ptr->numBottom, path);
}

void CircleProduct::updateLocateResult(const uint32_t number, const std::string &value)
{
    LogInfo("product process:\r\nupdate locate res:\r\nnumber={}, value={}.", number, value);
    std::lock_guard lock(mtxProduct_);
    for (const auto &ptr : lvProduct_)
    {
        if (ptr && ptr->numBottom == number)
        {
            ptr->locateResult = value;
        }
    }
}

void CircleProduct::updateLocateCheck(const cv::Mat &mat, const std::string &path)
{
    auto ptr = lvProduct_[OffsetLocateCheck];
    if (ptr == nullptr)
        return;
    std::lock_guard lock(mtxProduct_);
    ptr->locateCheckImage = mat;
    ptr->locateCheckImagePath = path;
    LogInfo("product process:\r\nupdate locate check:\r\nnumber={}, path={}.", ptr->numBottom, path);
}

void CircleProduct::updateLocateCheckResult(const uint32_t number, const std::string &value)
{
    LogInfo("product process:\r\nupdate locate check res:\r\nnumber={}, value={}.", number, value);
    std::lock_guard lock(mtxProduct_);
    for (const auto &ptr : lvProduct_)
    {
        if (ptr && ptr->numBottom == number)
        {
            ptr->locateCheckResult = value;
        }
    }
}

void CircleProduct::updateOCR(const cv::Mat &mat, const std::string &path)
{
    auto ptr = lvProduct_[OffsetOCR];
    if (ptr == nullptr)
        return;
    std::lock_guard lock(mtxProduct_);
    ptr->codeCheckImage = mat;
    ptr->codeCheckImagePath = path;
    LogInfo("product process:\r\nupdate ocr:\r\nnumber={}, path={}.", ptr->numBottom, path);
}

void CircleProduct::updateOCRResult(const uint32_t number, const std::string &value)
{
    LogInfo("product process:\r\nupdate ocr res:\r\nnumber={}, value={}.", number, value);
    std::lock_guard lock(mtxProduct_);
    for (const auto &ptr : lvProduct_)
    {
        if (ptr && ptr->numBottom == number)
        {
            ptr->OCRResult = value;
        }
    }
}

const std::deque<ProductItem *> &CircleProduct::getCircleProduct()
{
    return lvProduct_;
}

const ProductItem *CircleProduct::getNumber(const uint32_t number)
{
    for (const auto &ptr : lvProduct_)
    {
        if (ptr && ptr->numBottom == number)
        {
            return ptr;
        }
    }
    return nullptr;
}
