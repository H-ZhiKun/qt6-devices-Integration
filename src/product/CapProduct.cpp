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
    CapProductTimeWapper::insert(ptr);
    CapProductDataWapper::insert(ptr);
    return ptr;
}
