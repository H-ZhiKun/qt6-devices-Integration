#include "CircleProduct.h"

// CircleProduct::CircleProduct()
// {
//     lvProduct_.resize(24);
// }

// CircleProduct::~CircleProduct()
// {
// }

// void CircleProduct::newProduct(uint32_t number)
// {
//     auto ptr = std::make_shared<ProductItem>(number);
//     std::lock_guard lock(mtxProduct_);
//     lvProduct_.push_front(ptr);
// }

// void CircleProduct::completeProduct()
// {
//     std::lock_guard lock(mtxProduct_);
//     if (lvProduct_.size() == 0)
//         return;
//     auto ptr = lvProduct_.back();
//     if (ptr && ptr->isComplete)
//     {
//         LogInfo("product process:complete:number={},complete={}.", ptr->numBottom, ptr->isComplete);
//     }
//     lvProduct_.pop_back();
// }

// uint16_t CircleProduct::updateQRCode(const std::string &code)
// {
//     std::lock_guard lock(mtxProduct_);
//     auto ptr = lvProduct_[OffsetQRCode];
//     if (ptr == nullptr)
//         return 0;
//     ptr->QRCode = code;
//     LogInfo("product process:update QRCode:code={}.", code);
//     return ptr->numBottom;
// }

// void CircleProduct::updateLogistics(const uint32_t number, const std::string &logisticsDes, const std::string &code1,
//                                     const std::string &code2)
// {
//     LogInfo("product process:update logistics:number={}, code1={}, code2={}.", number, code1, code2);
//     std::lock_guard lock(mtxProduct_);
//     for (const auto ptr : lvProduct_)
//     {
//         if (ptr && ptr->numBottom == number)
//         {
//             if (!ptr->logistics1.empty())
//             {
//                 return;
//             }
//             ptr->logistics1 = code1;
//             ptr->logistics2 = code2;
//             ptr->logisticsDes = logisticsDes;
//         }
//     }
// }

// void CircleProduct::updateLocate(const cv::Mat &mat, const std::string &path)
// {
//     std::lock_guard lock(mtxProduct_);
//     auto ptr = lvProduct_[OffsetLocate];
//     if (ptr == nullptr)
//         return;
//     ptr->locateImage = mat;
//     ptr->locateImagePath = path;
//     LogInfo("product process:update locate:number={}, path={}.", ptr->numBottom, path);
// }

// void CircleProduct::updateLocateResult(const uint32_t number, const std::string &value)
// {
//     LogInfo("product process:update locate res:number={}, value={}.", number, value);
//     std::lock_guard lock(mtxProduct_);
//     for (const auto ptr : lvProduct_)
//     {
//         if (ptr && ptr->numBottom == number)
//         {
//             ptr->locateResult = value;
//         }
//     }
// }

// void CircleProduct::updateLocateCheck(const cv::Mat &mat, const std::string &path)
// {
//     std::lock_guard lock(mtxProduct_);
//     auto ptr = lvProduct_[OffsetLocateCheck];
//     if (ptr == nullptr)
//         return;
//     ptr->locateCheckImage = mat;
//     ptr->locateCheckImagePath = path;
//     LogInfo("product process:update locate check:number={}, path={}.", ptr->numBottom, path);
// }

// void CircleProduct::updateLocateCheckResult(const uint32_t number, const std::string &value)
// {
//     LogInfo("product process:update locate check res:number={}, value={}.", number, value);
//     std::lock_guard lock(mtxProduct_);
//     for (const auto ptr : lvProduct_)
//     {
//         if (ptr && ptr->numBottom == number)
//         {
//             ptr->locateCheckResult = value;
//         }
//     }
// }

// void CircleProduct::updateOCR(const cv::Mat &mat, const std::string &path)
// {
//     std::lock_guard lock(mtxProduct_);
//     auto ptr = lvProduct_[OffsetOCR];
//     if (ptr == nullptr)
//         return;
//     ptr->codeCheckImage = mat;
//     ptr->codeCheckImagePath = path;
//     LogInfo("product process:update ocr:number={}, path={}.", ptr->numBottom, path);
// }

// void CircleProduct::updateOCRResult(const uint32_t number, const std::string &value)
// {
//     LogInfo("product process:update ocr res:number={}, value={}.", number, value);
//     std::lock_guard lock(mtxProduct_);
//     for (const auto ptr : lvProduct_)
//     {
//         if (ptr && ptr->numBottom == number)
//         {
//             ptr->OCRResult = value;
//         }
//     }
// }

// std::shared_ptr<ProductItem> CircleProduct::getNumber(const uint32_t number)
// {
//     std::lock_guard lock(mtxProduct_);
//     for (const auto ptr : lvProduct_)
//     {
//         if (ptr && ptr->numBottom == number)
//         {
//             return ptr;
//         }
//     }
//     return nullptr;
// }

// std::shared_ptr<ProductItem> CircleProduct::getIndex(const uint32_t index)
// {
//     std::lock_guard lock(mtxProduct_);
//     if (index >= lvProduct_.size())
//     {
//         return nullptr;
//     }
//     return lvProduct_[index];
// }

void CircleProduct::signalQR(uint32_t pdNum)
{
    auto ptr = std::make_shared<ProductItem>(pdNum, pdType_, "", "");
    qProduct_.push_front(ptr);
}

void CircleProduct::signalLocation()
{
}

void CircleProduct::signalCheck()
{
}

std::shared_ptr<ProductItem> CircleProduct::signalCoding()
{
    return std::shared_ptr<ProductItem>();
}

void CircleProduct::signalOCR()
{
}

void CircleProduct::signalComplete()
{
}

void CircleProduct::updateQRCode(const std::string &code)
{
}

void CircleProduct::updateLogistics(const std::string &code1, const std::string &code2)
{
}

uint32_t CircleProduct::updateLocation(const cv::Mat &mat, const std::string &path)
{
    return 0;
}

std::shared_ptr<ProductItem> CircleProduct::updateLocationResult(const uint32_t number, const std::string &value)
{
    return std::shared_ptr<ProductItem>();
}

uint32_t CircleProduct::updateCheck(const cv::Mat &mat, const std::string &path)
{
    return 0;
}

std::shared_ptr<ProductItem> CircleProduct::updateCheckResult(const uint32_t number, const std::string &value)
{
    return std::shared_ptr<ProductItem>();
}

uint32_t CircleProduct::updateOCR(const cv::Mat &mat, const std::string &path)
{
    return 0;
}

std::shared_ptr<ProductItem> CircleProduct::updateOCRResult(const uint32_t number, const std::string &value)
{
    return std::shared_ptr<ProductItem>();
}

std::shared_ptr<ProductItem> CircleProduct::getIndexObject(uint32_t index)
{
    return std::shared_ptr<ProductItem>();
}
