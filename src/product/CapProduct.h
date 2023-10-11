#pragma once
#include "BaseProduct.h"
#include "CapProductDataWapper.h"
#include "CapProductTimeWapper.h"
#include "Logger.h"
#include "Utils.h"

class CapProduct : public BaseProduct
{
  public:
    explicit CapProduct();
    virtual ~CapProduct();
    // 不是ocr，是内塞检测
    virtual void signalOCR() override;
    virtual void signalComplete() override;
};