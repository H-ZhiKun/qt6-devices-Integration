#pragma once
#include "BaseProduct.h"

class CircleProduct : public BaseProduct
{
  public:
    explicit CircleProduct();
    virtual ~CircleProduct() = default;
    virtual void init(const YAML::Node &config = YAML::Node()) override;

  protected:
    virtual std::string storeRotatingQueue(std::shared_ptr<ProductItem>) override; // 存储旋转下发队列
    virtual std::string storePrintQueue(std::shared_ptr<ProductItem>) override;
    virtual std::string storeRemoveQueue(std::shared_ptr<ProductItem>) override;
};