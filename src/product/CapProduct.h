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

class CapProductData : public ProductData
{
  public:
    CapProductData() = default;
    ~CapProductData() = default;
    virtual void zeroClear()
    {
        countAll = 0;
        countPass = 0;
        countWaste = 0;
        countLocateWaste = 0;
        countCodeWaste = 0;
        countPauseWaste = 0;
    };
    uint32_t countLocateWaste = 0; // 定位废品数
    uint32_t countCodeWaste = 0;   // 喷码废品数
    uint32_t countPauseWaste = 0;  // 暂停/终止废品数
};