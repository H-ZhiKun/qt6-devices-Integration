#pragma once
#include "BaseProduct.h"

class ImpurityProduct : public BaseProduct
{
  public:
    explicit ImpurityProduct();
    virtual ~ImpurityProduct() = default;
    virtual void init(const YAML::Node &config = YAML::Node()) override;
};