#pragma once
#include "BaseProduct.h"
#include "Logger.h"
#include "ProductDataWapper.h"
#include "ProductTimeWapper.h"
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