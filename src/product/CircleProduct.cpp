#include "CircleProduct.h"

CircleProduct::CircleProduct()
{
    pdType_ = TypeProduct::TypeCircle;
}

void CircleProduct::signalQR(uint32_t pdNum)
{
    auto ptr = std::make_shared<ProductItem>(pdNum, TypeProduct::TypeCircle, "", "");
    qProduct_.push_front(ptr);
    if (qProduct_.size() > OffsetLocate && qProduct_[OffsetLocate]->bottleNum_ > 0)
    {
        qProduct_[OffsetLocate]->LocationSigTime = Utils::getCurrentTime(true);
    }
    if (qProduct_.size() > OffsetLocateCheck && qProduct_[OffsetLocateCheck]->bottleNum_ > 0)
    {
        qProduct_[OffsetLocateCheck]->CheckSigTime = Utils::getCurrentTime(true);
    }
    if (qProduct_.size() > OffsetCoding && qProduct_[OffsetCoding]->bottleNum_ > 0)
    {
        qProduct_[OffsetCoding]->codingSigTime = Utils::getCurrentTime(true);
    }
    if (qProduct_.size() > OffsetOCR && qProduct_[OffsetOCR]->bottleNum_ > 0)
    {
        qProduct_[OffsetOCR]->OCRSigTime = Utils::getCurrentTime(true);
    }
}

void CircleProduct::signalComplete()
{
    if (qProduct_.size() == 0)
        return;
    auto ptr = qProduct_.back();
    if (ptr->bottleNum_ > 0)
    {
        ptr->completeSigTime = Utils::getCurrentTime(true);
        CircleProductTimeWapper::insert(ptr);
        CircleProductDataWapper::insert(ptr);
    }
    qProduct_.pop_back();
}

void CircleProduct::updateQRCode(const std::string &code)
{
    auto ptr = qProduct_[OffsetQRCode];
    if (ptr->bottleNum_ == 0)
        return;
    ptr->QRCode = code;
    ptr->QRCodeTime = Utils::getCurrentTime(true);
}

uint32_t CircleProduct::updateLocation(const cv::Mat &mat, const std::string &path)
{
    auto ptr = qProduct_[OffsetLocate];
    if (ptr->bottleNum_ == 0)
        return 0;
    ptr->LocationImage = mat;
    ptr->LocationPath = path;
    ptr->LocationImageTime = Utils::getCurrentTime(true);
}

uint32_t CircleProduct::updateCheck(const cv::Mat &mat, const std::string &path)
{
    auto ptr = qProduct_[OffsetLocateCheck];
    if (ptr->bottleNum_ == 0)
        return 0;
    ptr->CheckImage = mat;
    ptr->CheckPath = path;
    ptr->CheckImageTime = Utils::getCurrentTime(true);
}

uint32_t CircleProduct::updateOCR(const cv::Mat &mat, const std::string &path)
{
    auto ptr = qProduct_[OffsetOCR];
    if (ptr->bottleNum_ == 0)
        return 0;
    ptr->OCRImage = mat;
    ptr->OCRPath = path;
    ptr->OCRImageTime = Utils::getCurrentTime(true);
}
