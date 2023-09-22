#pragma once
#include "BaseProduct.h"
#include "Logger.h"
#include "Utils.h"
#include <memory>
#include <mutex>
#include <vector>
class CircleProduct : public BaseProduct
{
  public:
    explicit CircleProduct() = default;
    virtual ~CircleProduct() = default;
    virtual void signalQR(uint32_t pdNum = 0) override;
    virtual void signalLocation() override;
    virtual void signalCheck() override;
    virtual std::shared_ptr<ProductItem> signalCoding() override;
    virtual void signalOCR() override;
    virtual void signalComplete() override;

    virtual void updateQRCode(const std::string &code) override;
    virtual void updateLogistics(const std::string &code1, const std::string &code2) override;
    virtual uint32_t updateLocation(const cv::Mat &mat, const std::string &path) override;
    virtual std::shared_ptr<ProductItem> updateLocationResult(const uint32_t number, const std::string &value) override;
    virtual uint32_t updateCheck(const cv::Mat &mat, const std::string &path) override;
    virtual std::shared_ptr<ProductItem> updateCheckResult(const uint32_t number, const std::string &value) override;
    virtual uint32_t updateOCR(const cv::Mat &mat, const std::string &path) override;
    virtual std::shared_ptr<ProductItem> updateOCRResult(const uint32_t number, const std::string &value) override;
    virtual std::shared_ptr<ProductItem> getIndexObject(uint32_t index) override;
    virtual void complete() override;

  private:
    std::deque<std::shared_ptr<ProductItem>> qProduct_;
    TypeProduct pdType_{TypeProduct::TypeCircle};
};