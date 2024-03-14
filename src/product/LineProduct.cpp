#include "LineProduct.h"
#include "AppTest.h"
#include <Logger.h>
LineProduct::LineProduct()
{
    pdType_ = TypeProduct::TypeLine;
}

std::string LineProduct::storePrintQueue(std::shared_ptr<ProductItem> product)
{
    auto pos = printIndex_ % printQueueLength_;
    ++printIndex_;
    auto code = product->getValue<std::string>(ProductItemKey::logistics);
    if (code.size() == 24)
    {
        printQueue_[pos] = true;
    }
    else
    {
        printQueue_[pos] = false;
    }
    return fmt::format("[{},{}]", pos, printQueue_[pos]);
}

std::string LineProduct::storeRemoveQueue(std::shared_ptr<ProductItem> product)
{
    auto pos = removeIndex_ % removeQueueLength_;
    ++removeIndex_;
#ifdef APP_TEST
    // removeQueue_[pos] = (pos % 2) == 0 ? false : true;
    removeQueue_[pos] = true;
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
