#pragma once
#include "BaseProduct.h"
#include "LineProductDataWapper.h"
#include "LineProductTimeWapper.h"
#include "Logger.h"
#include "Utils.h"

class CapProduct : public BaseProduct
{
  public:
    explicit CapProduct();
    virtual ~CapProduct();
    virtual std::shared_ptr<ProductItem> signalCoding() override;
    // 不是ocr，是内塞检测
    virtual void signalOCR() override;
    virtual void signalComplete() override;
};