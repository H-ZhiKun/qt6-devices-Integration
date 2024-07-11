#pragma once
#include "CapProduct.h"

class LineProduct : public CapProduct
{
  public:
    explicit LineProduct();
    virtual ~LineProduct() = default;
};