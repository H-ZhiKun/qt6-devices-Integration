#include "BaseProduct.h"
#include "Logger.h"
#include "ProductWapper.h"
#include <cstdint>
#include <fmt/core.h>
#include <qimage.h>

BaseProduct::BaseProduct(QObject *parent) : pdType_(TypeProduct::TypeBase), QObject(parent)
{
}

const TypeProduct &BaseProduct::getType()
{
    return pdType_;
}

void BaseProduct::init(const YAML::Node &config)
{
    rotatingQueue_.resize(rotatingQueueLength_);
    printQueue_.resize(printQueueLength_);
    removeQueue_.resize(removeQueueLength_);
    reRotatingQueue_.resize(reRotatingQueueLength_);
    // plcIds_ = {{keyStore_.bottle_num, 0},
    //            {keyStore_.qr_code, 0},
    //            {keyStore_.location_path, 0},
    //            {keyStore_.check_path, 0},
    //            {keyStore_.ocr_path, 0}};
}

SubType BaseProduct::getSubType() const
{
    return subType_;
}

BottleShape BaseProduct::getBottleShape() const
{
    return bottleShape_;
}

void BaseProduct::setBottleShape(BottleShape shape)
{
    bottleShape_ = shape;
}

int BaseProduct::getOffset() const
{
    if (bottleShape_ == BottleShape::Round)
    {
        return roundOffset_;
    }
    else
    {
        return squareOffset_;
    }
}

void BaseProduct::createProduct(uint16_t count)
{
    std::lock_guard lock(mtxProduct_);
    while (count > 0)
    {
        --count;
        ++productId;
        auto pd = std::make_shared<ProductItem>(productId);
        qProduct_.push_front(pd);
        LogInfo("product id={},size={} created", productId, qProduct_.size());
        if (qProduct_.size() > 64)
        {
            auto ptr = qProduct_.back();
            LogInfo("product id={},size={}, out of 64", ptr->bottleNumber(), qProduct_.size());
            ProductWapper::insert(getType(), ptr);
            qProduct_.pop_back();
        }
    }
}

std::shared_ptr<ProductItem> BaseProduct::updateValue(const uint32_t productId, const std::string &key,
                                                      const ValueVariant &value)
{
    std::lock_guard lock(mtxProduct_);

    auto findProductById = [&productId](const auto &product) -> bool { return product->bottleNumber() == productId; };
    auto iter = std::find_if(qProduct_.begin(), qProduct_.end(), findProductById);
    if (iter != qProduct_.end())
    {
        auto product = *iter;
        product->setValue(key, value);
        judgment(key, value, product);
        return product;
    }
    LogInfo("product id={} not found, key={}", productId, key);
    return nullptr;
}

void BaseProduct::judgment(const std::string &key, const ValueVariant &value,
                           const std::shared_ptr<ProductItem> &product)
{
    if (key == ProductItemKey::qr_code)
    {
        auto condition = std::get<std::string>(value);
        if (condition == "no read")
            product->setRemoveTrue();
    }
    else if (key == ProductItemKey::logistics)
    {
        auto condition = std::get<std::string>(value);
        if (condition.size() != 24)
        {
            product->setRemoveTrue();
            product->setPrintFalse();
        }
    }
    else if (key == ProductItemKey::check_result)
    {
        auto condition = std::get<std::string>(value);
        if (condition == "361")
        {
            product->setRemoveTrue();
            product->setPrintFalse();
        }
    }
    else if (key == ProductItemKey::ocr_result)
    {
        auto condition = std::get<std::string>(value);
        std::string tempValue;
        if (getType() == TypeProduct::TypeCap)
        {
            tempValue = condition == "1" ? "true" : "false";
        }
        else
        {
            tempValue = condition == product->getValue<std::string>(ProductItemKey::logistics) ? "true" : "false";
        }
        if (tempValue == "false")
            product->setRemoveTrue();
        product->setValue(ProductItemKey::is_ocr_equal_code, tempValue);
    }
}

std::list<std::shared_ptr<ProductItem>> BaseProduct::deleteProduct(uint16_t count)
{
    std::lock_guard lock(mtxProduct_);
    std::list<std::shared_ptr<ProductItem>> lvRet;
    while (count > 0)
    {
        if (qProduct_.empty())
        {
            LogError("product id={}, delete empty", count);
            return lvRet;
        }
        --count;
        auto ptr = qProduct_.back();
        ProductWapper::insert(getType(), ptr);
        lvRet.push_back(ptr);
        qProduct_.pop_back();
        LogInfo("product id={},size={} delete", ptr->bottleNumber(), qProduct_.size());
    }
    return lvRet;
}

std::string BaseProduct::storeRotatingQueue(const std::string &value)
{
    auto pos = rotatingIndex_ % rotatingQueueLength_;
    ++rotatingIndex_;
    auto tempFloat = std::stof(value);
    auto rotateValue = tempFloat * 100.0f;
    rotatingQueue_[pos] = rotateValue;
    return fmt::format("[{},{}]", pos, rotateValue);
}

std::string BaseProduct::storePrintQueue(uint8_t flag)
{
    auto pos = printIndex_ % printQueueLength_;
    ++printIndex_;
    printQueue_[pos] = flag;
    return fmt::format("[{},{}]", pos, flag);
}

std::string BaseProduct::storeRemoveQueue(uint8_t flag)
{
    auto pos = removeIndex_ % removeQueueLength_;
    ++removeIndex_;
    removeQueue_[pos] = flag;
    return fmt::format("[{},{}]", pos, flag);
}

std::string BaseProduct::storeReRotatingQueue(const std::string &value)
{
    auto pos = reRotatingIndex_ % reRotatingQueueLength_;
    ++reRotatingIndex_;
    auto tempFloat = std::stof(value);
    auto rotateValue = tempFloat * 100.0f;
    reRotatingQueue_[pos] = rotateValue;
    return fmt::format("[{},{}]", pos, rotateValue);
}

void BaseProduct::clearQueue()
{
    std::lock_guard lock(mtxProduct_);
    rotatingIndex_ = 0;
    printIndex_ = 0;
    removeIndex_ = 0;
    reRotatingIndex_ = 0;
    rotatingQueue_.assign(rotatingQueue_.size(), 0);
    printQueue_.assign(printQueue_.size(), 0);
    removeQueue_.assign(removeQueue_.size(), 0);
    reRotatingQueue_.assign(reRotatingQueue_.size(), 0);
}

void BaseProduct::clearProductId()
{
    productId = 0;
}
void BaseProduct::setAllRemove()
{
    for (auto &product : qProduct_)
    {
        product->setRemoveTrue();
    }
}