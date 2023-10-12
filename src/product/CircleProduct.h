#pragma once
#include "BaseProduct.h"
#include "CircleProductDataWapper.h"
#include "CircleProductTimeWapper.h"
#include "Logger.h"
#include "Utils.h"
#include <memory>
#include <mutex>
#include <vector>
class CircleProduct : public BaseProduct
{
  public:
    explicit CircleProduct();
    virtual ~CircleProduct() = default;
    virtual void createProduct(uint32_t pdNum, const std::string &batchNum = "",
                               const std::string &formulaName = "") override;
    virtual std::shared_ptr<ProductItem> deleteProduct() override;

  private:
    uint16_t OffsetQRCode = 0;
    uint16_t OffsetLocate = 3;
    uint16_t OffsetCheck = 6;
    uint16_t OffsetCoding = 9;
    uint16_t OffsetOCR = 14;
};

class CircleProductData : public ProductData
{
  public:
    CircleProductData() = default;
    ~CircleProductData() = default;
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
