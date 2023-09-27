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