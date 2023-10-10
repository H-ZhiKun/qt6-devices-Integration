#pragma once
#include "CapProduct.h"

CapProduct::CapProduct()
{
    pdType_ = TypeProduct::TypeCap;
}

CapProduct::~CapProduct()
{
}

std::shared_ptr<ProductItem> CapProduct::signalCoding()
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

void CapProduct::signalOCR()
{
    for (auto ptr = qProduct_.rbegin(); ptr != qProduct_.rend(); ++ptr)
    {
        if ((*ptr)->bottleNum_ > 0 && (*ptr)->OCRSigTime.empty())
        {
            (*ptr)->OCRSigTime = Utils::getCurrentTime(true);
            return;
        }
    }
}

void CapProduct::signalComplete()
{
    LogInfo("CapProduct signalComplete");
    auto ptr = qProduct_.back();
    // 插入数据库
    ptr->completeSigTime = Utils::getCurrentTime(true);
    CapProductTimeWapper::insert(ptr);
    CapProductDataWapper::insert(ptr);
    qProduct_.pop_back();
}
