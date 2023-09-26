#pragma once
#include "LineProduct.h"

LineProduct::LineProduct()
{
    pdType_ = TypeProduct::TypeLine;
}

LineProduct::~LineProduct()
{
}

void LineProduct::signalQR(uint32_t pdNum)
{
    ++curBottleNum_;
    auto pd = std::make_shared<ProductItem>(curBottleNum_, pdType_, "", "");
    pd->QRSigTime = Utils::getCurrentTime(true);
    qProduct_.push_front(pd);
}

void LineProduct::signalLocation()
{
}

void LineProduct::signalCheck()
{
}

std::shared_ptr<ProductItem> LineProduct::signalCoding()
{
    for (auto ptr = qProduct_.rbegin(); ptr != qProduct_.rend(); ++ptr)
    {
        if ((*ptr)->bottleNum_ > 0 && (*ptr)->codingSigTime.empty())
        {
            (*ptr)->codingSigTime = Utils::getCurrentTime(true);
            return (*ptr);
        }
    }
    return nullptr;
}

void LineProduct::signalOCR()
{
    for (auto ptr = qProduct_.rbegin(); ptr != qProduct_.rend(); ++ptr)
    {
        if ((*ptr)->bottleNum_ > 0 && (*ptr)->OCRSigTime.empty())
        {
            (*ptr)->OCRSigTime = Utils::getCurrentTime(true);
        }
    }
}

void LineProduct::signalComplete()
{
    LogInfo("LineProduct signalComplete");
    auto ptr = qProduct_.back();
    // 插入数据库
    ptr->completeSigTime = Utils::getCurrentTime(true);
    ProductTimeWapper::insert(ptr);
    ProductDataWapper::insert(ptr);
    qProduct_.pop_back();
}
