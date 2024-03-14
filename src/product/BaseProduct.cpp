#include "BaseProduct.h"
#include "Logger.h"
#include "ProductWapper.h"
#include <cstdint>
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
    // uint32_t usedId = productId;
    // if (key == keyStore_.qr_code || key == keyStore_.location_path || key == keyStore_.check_path ||
    //     key == keyStore_.ocr_path)
    // {
    //     usedId = plcIds_.at(key) + 1;
    // }

    auto findProductById = [&productId](const auto &product) -> bool { return product->bottleNumber() == productId; };
    auto iter = std::find_if(qProduct_.begin(), qProduct_.end(), findProductById);
    if (iter != qProduct_.end())
    {
        auto product = *iter;
        product->setValue(key, value);
        // if (key == keyStore_.qr_code || key == keyStore_.location_path || key == keyStore_.check_path ||
        //     key == keyStore_.ocr_path)
        // {
        //     plcIds_.at(key) = usedId;
        //     LogInfo("product id check,setValue qrcode={},location={},check={},ocr={}", plcIds_.at(keyStore_.qr_code),
        //             plcIds_.at(keyStore_.location_path), plcIds_.at(keyStore_.check_path),
        //             plcIds_.at(keyStore_.ocr_path));
        // }
        // else if (key == keyStore_.ocr_result)
        // {
        //     std::string code = std::get<std::string>(value);
        //     std::string compare =
        //         code == product->getValue<std::string>(BaseProduct::keyStore_.logistics) ? "true" : "false";
        //     product->setValue(keyStore_.is_ocr_equal_code, compare);
        // }
        return product;
    }
    return nullptr;
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

std::string BaseProduct::storeRotatingQueue(std::shared_ptr<ProductItem>)
{
    return std::string();
}

std::string BaseProduct::storePrintQueue(std::shared_ptr<ProductItem>)
{
    return std::string();
}

std::string BaseProduct::storeRemoveQueue(std::shared_ptr<ProductItem>)
{
    return std::string();
}

void BaseProduct::clearQueue()
{
    std::lock_guard lock(mtxProduct_);
    rotatingIndex_ = 0;
    printIndex_ = 0;
    removeIndex_ = 0;
}

void BaseProduct::clearProductId()
{
    productId = 0;
}
void BaseProduct::setAllRemove()
{
    for (auto &product : qProduct_)
    {
        product->setRemove();
    }
}