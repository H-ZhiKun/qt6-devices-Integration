#include "CircleProduct.h"
#include "AppTest.h"
#include "BaseProduct.h"
#include <Logger.h>
#include <string>

CircleProduct::CircleProduct()
{
    pdType_ = TypeProduct::TypeCircle;
}

void CircleProduct::init(const YAML::Node &config)
{
    std::string strSubType = config["product"]["sub_type"].as<std::string>();
    if (strSubType == "antiClockWise")
    {
        subType_ = SubType::AntiClockWise;
    }
    else
    {
        subType_ = SubType::ClockWise;
    }
    roundOffset_ = config["product"]["round_offset"].as<int>();
    squareOffset_ = config["product"]["square_offset"].as<int>();
    BaseProduct::init(config);
}

std::string CircleProduct::storeRotatingQueue(std::shared_ptr<ProductItem> product)
{
    // number从0开始
    auto pos = rotatingIndex_ % rotatingQueueLength_;
    ++rotatingIndex_;
    auto tempFloat = std::stof(product->getValue<std::string>(ProductItemKey::location_result));
    auto rotateValue = tempFloat * 100.0f;
    rotatingQueue_[pos] = rotateValue;
    return fmt::format("[{},{}]", pos, rotateValue);
}

std::string CircleProduct::storePrintQueue(std::shared_ptr<ProductItem> product)
{
    auto pos = printIndex_ % printQueueLength_;
    ++printIndex_;
    auto result = product->getValue<std::string>(ProductItemKey::check_result);
    auto code = product->getValue<std::string>(ProductItemKey::logistics);
    if (result == "1" && code.size() == 24)
    {
        printQueue_[pos] = true;
    }
    else
    {
        printQueue_[pos] = false;
    }
    return fmt::format("[{},{}]", pos, printQueue_[pos]);
}

std::string CircleProduct::storeRemoveQueue(std::shared_ptr<ProductItem> product)
{
    auto pos = removeIndex_ % removeQueueLength_;
    ++removeIndex_;
#ifdef APP_TEST
    removeQueue_[pos] = false;
    return fmt::format("[{},{}]", pos, false);
#endif
    std::string qrcode = product->getValue<std::string>(ProductItemKey::qr_code);
    std::string checkResult = product->getValue<std::string>(ProductItemKey::check_result);
    std::string logistics = product->getValue<std::string>(ProductItemKey::logistics);
    std::string ocrResult = product->getValue<std::string>(ProductItemKey::ocr_result);
    if (product->isRemove() || qrcode == "no read" || logistics.size() != 24 || checkResult != "1" ||
        logistics != ocrResult)
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
