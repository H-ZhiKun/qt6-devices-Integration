#pragma once
#include "LineProduct.h"

LineProduct::LineProduct()
{
    pdType_ = TypeProduct::TypeLine;
}

LineProduct::~LineProduct()
{
}

void LineProduct::createProduct(uint32_t pdNum, const std::string &batchNum, const std::string &formulaName)
{
    ++customNum_;
    BaseProduct::createProduct(customNum_, batchNum, formulaName);
}

std::shared_ptr<ProductItem> LineProduct::deleteProduct()
{
    auto ptr = BaseProduct::deleteProduct();
    if (ptr->bottleNum_ > 0)
    {
        LineProductTimeWapper::insert(ptr);
        LineProductDataWapper::insert(ptr);
    }
    return ptr;
}

LineCount::LineCount()
{
    countData["countAll"] = 0;
    countData["countPass"] = 0;
    countData["countWaste"] = 0;
    countData["countCodeWaste"] = 0; // 喷码废品数
}
