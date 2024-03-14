#pragma once
#include "CapProduct.h"

class LineProduct : public CapProduct
{
  public:
    explicit LineProduct();
    virtual ~LineProduct() = default;

  protected:
    virtual std::string storePrintQueue(std::shared_ptr<ProductItem>) override;
    virtual std::string storeRemoveQueue(std::shared_ptr<ProductItem>) override;
};