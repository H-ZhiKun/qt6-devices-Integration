#pragma once
#include "BaseProduct.h"
#include "Logger.h"
#include "ProductDataWapper.h"
#include "ProductTimeWapper.h"
#include "Utils.h"
#include <memory>
#include <mutex>
#include <vector>
class CircleProduct : public BaseProduct
{
  public:
    explicit CircleProduct();
    virtual ~CircleProduct() = default;
    virtual void signalQR(uint32_t pdNum = 0) override;
    virtual void signalLocation() override;
    virtual void signalCheck() override;
    virtual std::shared_ptr<ProductItem> signalCoding() override;
    virtual void signalOCR() override;
    virtual void signalComplete() override;

  private:
    uint16_t OffsetQRCode = 0;
    uint16_t OffsetLocate = 3;
    uint16_t OffsetLocateCheck = 6;
    uint16_t OffsetOCR = 14;
};