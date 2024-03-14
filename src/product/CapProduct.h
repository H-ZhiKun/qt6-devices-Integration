#pragma once
#include "BaseProduct.h"

class CapProduct : public BaseProduct
{
  public:
    explicit CapProduct();
    virtual ~CapProduct();

  protected:
    virtual std::string storeRemoveQueue(std::shared_ptr<ProductItem>) override;
};