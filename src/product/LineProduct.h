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
    std::mutex mtxQR_;
    std::mutex mtxCoding_;
    std::mutex mtxOCR_;
    std::list<std::shared_ptr<ProductItem>> lvQR_;
    std::list<std::shared_ptr<ProductItem>> lvCoding_;
    std::list<std::shared_ptr<ProductItem>> lvOCR_;
    uint32_t curBottleNum_{0};
    uint32_t completeBitValue_ = 0;
    TypeProduct pdType_{TypeProduct::TypeLine};
};