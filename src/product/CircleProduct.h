#pragma once
#include "BaseProduct.h"
#include "Logger.h"
#include "CircleProductDataWapper.h"
#include "CircleProductTimeWapper.h"
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
    virtual void signalComplete() override;

    virtual void updateQRCode(const std::string &code) override;
    virtual uint32_t updateLocation(const cv::Mat &mat, const std::string &path) override;
    virtual uint32_t updateCheck(const cv::Mat &mat, const std::string &path) override;
    virtual uint32_t updateOCR(const cv::Mat &mat, const std::string &path) override;

  private:
    uint16_t OffsetQRCode = 0;
    uint16_t OffsetLocate = 3;
    uint16_t OffsetLocateCheck = 6;
    uint16_t OffsetCoding = 9;
    uint16_t OffsetOCR = 14;
};