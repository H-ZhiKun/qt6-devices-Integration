#pragma once
#include "BaseProduct.h"
#include "LineProductDataWapper.h"
#include "LineProductTimeWapper.h"
#include "Logger.h"
#include "Utils.h"

class LineProduct : public BaseProduct
{
  public:
    explicit LineProduct();
    virtual ~LineProduct();
    virtual std::shared_ptr<ProductItem> signalCoding() override;
    virtual void signalOCR() override;
    virtual void signalComplete() override;
};

class LineProductData : public ProductData
{
  public:
    LineProductData() = default;
    ~LineProductData() = default;
    virtual void zeroClear()
    {
        countAll = 0;
        countPass = 0;
        countWaste = 0;
        countCodeWaste = 0;
    };
    uint32_t countCodeWaste = 0; // 喷码废品数
};