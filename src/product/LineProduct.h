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
    virtual void createProduct(uint32_t pdNum, const std::string &batchNum = "",
                               const std::string &formulaName = "") override;
    virtual std::shared_ptr<ProductItem> deleteProduct() override;

  protected:
    uint32_t customNum_ = 0;
};

class LineCount : public Count
{
  public:
    LineCount();
    ~LineCount() = default;
    uint32_t countCodeWaste = 0; // 喷码废品数
};