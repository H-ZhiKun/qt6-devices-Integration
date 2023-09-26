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
    virtual void signalQR(uint32_t pdNum = 0) override;
    virtual void signalLocation() override;
    virtual void signalCheck() override;
    virtual std::shared_ptr<ProductItem> signalCoding() override;
    virtual void signalOCR() override;
    virtual void signalComplete() override;

  private:
    uint32_t completeBitValue_ = 0;
};