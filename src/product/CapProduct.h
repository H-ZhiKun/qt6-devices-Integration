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
    virtual void createProduct(uint32_t pdNum, const std::string &batchNum = "",
                               const std::string &formulaName = "") override;
    virtual std::shared_ptr<ProductItem> deleteProduct() override;

  protected:
    uint32_t customNum_ = 0;
};