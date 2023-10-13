#pragma once
#include "CapProduct.h"

CapProduct::CapProduct()
{
    pdType_ = TypeProduct::TypeCap;
}

CapProduct::~CapProduct()
{
}

void CapProduct::createProduct(uint32_t pdNum, const std::string &batchNum, const std::string &formulaName)
{
}

std::shared_ptr<ProductItem> CapProduct::deleteProduct()
{
    LogInfo("CapProduct Complete");
    auto ptr = BaseProduct::deleteProduct();
    if (ptr->bottleNum_ > 0)
    {
        CapProductTimeWapper::insert(ptr);
        CapProductDataWapper::insert(ptr);
    }
    return ptr;
}

CapCount::CapCount()
{
    countData["countAll"] = 0;   // 进瓶数
    countData["countPass"] = 0;  // 合格品数
    countData["countWaste"] = 0; // 废品总数
    countData["countLocateWaste"] = 0;
    countData["countCodeWaste"] = 0;
    countData["countPauseWaste"] = 0;
}
