#pragma once
#include "BaseProduct.h"
#include "PgsqlHelper.h"

class ProductWapper
{
  public:
    ProductWapper() = default;
    ~ProductWapper() = default;
    static bool insert(TypeProduct type, std::shared_ptr<ProductItem> ptr)
    {
        std::list<std::string> needKey{
            ProductItemKey::qr_code.c_str(),          ProductItemKey::logistics.c_str(),
            ProductItemKey::location_result.c_str(),  ProductItemKey::check_result.c_str(),
            ProductItemKey::ocr_result.c_str(),       ProductItemKey::coding_values.c_str(),
            ProductItemKey::remove_values.c_str(),    ProductItemKey::rotating_values.c_str(),
            ProductItemKey::is_ocr_equal_code.c_str()};
        QVariantMap mapData;

        // mapData.insert(QString::fromStdString(ProductItemKey::formula_name), formula.c_str());
        mapData.insert(ProductItemKey::bottle_num.c_str(), ptr->bottleNumber());
        mapData.insert(ProductItemKey::is_remove.c_str(), ptr->isRemove());

        std::string sqlLocation = ptr->getValue<ProductImgaeData>(ProductItemKey::location_path).path_;
        std::string sqlCheck = ptr->getValue<ProductImgaeData>(ProductItemKey::check_path).path_;
        std::string sqlOCR = ptr->getValue<ProductImgaeData>(ProductItemKey::ocr_path).path_;
        std::string sqlOCRRes = ptr->getValue<std::string>(ProductItemKey::is_ocr_equal_code);
        mapData.insert(ProductItemKey::location_path.c_str(), sqlLocation.c_str());
        mapData.insert(ProductItemKey::check_path.c_str(), sqlCheck.c_str());
        mapData.insert(ProductItemKey::ocr_path.c_str(), sqlOCR.c_str());
        for (const auto &key : needKey)
        {
            std::string sqlTemp = ptr->getValue<std::string>(key);
            mapData.insert(key.c_str(), sqlTemp.c_str());
        }
        PgsqlHelper::getSqlHelper().insertData("product_data", mapData);
        return true;
    }

    static Json::Value selectProductDataPage()
    {
        return PgsqlHelper::getSqlHelper().selectDataPaged("product_data", 5, 1, "", "id desc");
    }

    static Json::Value selectCountData()
    {
        int countAll = PgsqlHelper::getSqlHelper().selectCount("product_data", "created_time >= CURRENT_DATE");
        int countWaste = PgsqlHelper::getSqlHelper().selectCount(
            "product_data", "created_time >= CURRENT_DATE AND is_remove = true"); // 剔瓶数量

        // int countPass = countAll - countWaste;
        int countPass = 0;
        Json::Value countJson;
        countJson["countAll"] = countAll;
        countJson["countWaste"] = countAll;
        countJson["countPass"] = countPass;
        return countJson;
    }

    static Json::Value selectDetailCountData()
    {
        int countAll = PgsqlHelper::getSqlHelper().selectCount("product_data", "created_time >= CURRENT_DATE");
        int countQRCodeWaste = PgsqlHelper::getSqlHelper().selectCount(
            "product_data", "created_time >= CURRENT_DATE AND qr_code = 'no read'"); // 扫码器扫码失败次数
        int countLocateWaste = PgsqlHelper::getSqlHelper().selectCount(
            "product_data", "created_time >= CURRENT_DATE AND check_result != '1'"); // 定位废品
        int countCodeWaste = PgsqlHelper::getSqlHelper().selectCount(
            "product_data", "created_time >= CURRENT_DATE AND is_ocr_equal_code != TRUE"); // 打码废品
        // int countPauseWaste = countWaste - countQRCodeWaste - countLocateWaste - countCodeWaste;
        int countQrCode =
            countAll - PgsqlHelper::getSqlHelper().selectCount(
                           "product_data", "created_time >= CURRENT_DATE AND qr_code IS NULL"); // 扫码器触发次数
        int countLocate = countAll - PgsqlHelper::getSqlHelper().selectCount(
                                         "product_data",
                                         "created_time >= CURRENT_DATE AND location_path IS NULL"); // 定位相机触发次数
        int countCheck =
            countAll - PgsqlHelper::getSqlHelper().selectCount(
                           "product_data", "created_time >= CURRENT_DATE AND check_path IS NULL"); // 复核相机触发次数
        int countOCR =
            countAll - PgsqlHelper::getSqlHelper().selectCount(
                           "product_data", "created_time >= CURRENT_DATE AND ocr_path IS NULL"); // OCR相机触发次数
        int countPauseWaste = 0;
        int QRCodeCollectFaile = 0;
        Json::Value countJson;
        countJson["countQRCodeWaste"] = countQRCodeWaste;
        countJson["countLocateWaste"] = countLocateWaste;
        countJson["countCodeWaste"] = countCodeWaste;
        countJson["countPauseWaste"] = countPauseWaste;
        countJson["countQrCode"] = countQrCode;
        countJson["countLocate"] = countLocate;
        countJson["countCheck"] = countCheck;
        countJson["countOCR"] = countOCR;
        return countJson;
    }

    static int productNum()
    {
        QString res =
            PgsqlHelper::getSqlHelper().selectOneData("product_data", "count(*)", "created_time::date = CURRENT_DATE");
        return res.toInt();
    }

    static Json::Value selectProductDataPaged(const int pageSize, const int pageNumber,
                                              const std::string &&condition = "", const std::string &&orderBy = "")
    {
        return PgsqlHelper::getSqlHelper().selectDataPaged("product_data", pageSize, pageNumber, std::move(condition),
                                                           std::move(orderBy));
    }
};