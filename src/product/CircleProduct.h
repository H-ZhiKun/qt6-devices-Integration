#pragma once
#include "BaseProduct.h"

class CircleProduct : public BaseProduct
{
  public:
    explicit CircleProduct();
    virtual ~CircleProduct() = default;
    virtual void init(const YAML::Node &config = YAML::Node()) override;
};