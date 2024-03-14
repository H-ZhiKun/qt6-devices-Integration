#include "CapProduct.h"
#include "AppTest.h"
#include <Logger.h>

CapProduct::CapProduct()
{
    pdType_ = TypeProduct::TypeCap;
}

CapProduct::~CapProduct()
{
}

std::string CapProduct::storeRemoveQueue(std::shared_ptr<ProductItem> product)
{
    auto pos = removeIndex_ % removeQueueLength_;
    ++removeIndex_;
#ifdef APP_TEST
    // removeQueue_[pos] = (pos % 2) == 0 ? false : true;
    removeQueue_[pos] = false;
    return fmt::format("[{},{}]", pos, removeQueue_[pos]);
#endif
    std::string qrcode = product->getValue<std::string>(ProductItemKey::qr_code);
    std::string logistics = product->getValue<std::string>(ProductItemKey::logistics);
    std::string ocrResult = product->getValue<std::string>(ProductItemKey::ocr_result);

    if (product->isRemove() || qrcode == "no read" || logistics.size() != 24 || ocrResult != logistics)
    {
        product->setRemove();
        removeQueue_[pos] = true;
    }
    else
    {
        removeQueue_[pos] = false;
    }
    return fmt::format("[{},{}]", pos, removeQueue_[pos]);
}
