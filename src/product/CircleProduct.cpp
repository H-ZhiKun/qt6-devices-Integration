#include "CircleProduct.h"

CircleProduct::CircleProduct()
{
    pdType_ = TypeProduct::TypeCircle;
}

void CircleProduct::createProduct(uint32_t pdNum, const std::string &batchNum, const std::string &formulaName)
{
    BaseProduct::createProduct(pdNum, batchNum, formulaName);
    if (qProduct_.size() > OffsetQRCode && qProduct_[OffsetQRCode]->bottleNum_ > 0)
    {
        signalQR();
    }
    if (qProduct_.size() > OffsetLocate && qProduct_[OffsetLocate]->bottleNum_ > 0)
    {
        signalLocation();
    }
    if (qProduct_.size() > OffsetCheck && qProduct_[OffsetCheck]->bottleNum_ > 0)
    {
        signalCheck();
    }
    if (qProduct_.size() > OffsetOCR && qProduct_[OffsetOCR]->bottleNum_ > 0)
    {
        signalOCR();
    }
}

std::shared_ptr<ProductItem> CircleProduct::deleteProduct()
{
    auto ptr = BaseProduct::deleteProduct();
    if (ptr->bottleNum_ > 0)
    {
        CircleProductTimeWapper::insert(ptr);
        CircleProductDataWapper::insert(ptr);
    }
    return ptr;
}