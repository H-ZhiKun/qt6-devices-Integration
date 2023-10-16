#pragma once
#include "BaseProduct.h"
#include "PgsqlHelper.h"
#include "Utils.h"
#include "json/json.h"
#include <QDebug>
#include <map>
#include <memory>
#include <string>

class CircleProductDataWapper
{
#define TABLE_PRODUCT_DATA "product_data"
  public:
    CircleProductDataWapper() = default;
    ~CircleProductDataWapper() = default;
    static bool insert(std::shared_ptr<ProductItem> ptr)
    {
        QVariantMap mapData;
        mapData["type_pd"] = static_cast<uint8_t>(ptr->typePd_);
        mapData["bottle_num"] = ptr->bottleNum_;
        mapData["batch_num"] = ptr->batchNum_.c_str();
        mapData["formula_name"] = ptr->formulaName_.c_str();
        mapData["is_remove"] = ptr->isRemove_;
        mapData["qrcode_result"] = ptr->QRCode.c_str();
        mapData["logistics_true_value_1"] = ptr->logistics1.c_str();
        mapData["logistics_true_value_2"] = ptr->logistics2.c_str();
        mapData["location_image_path"] = ptr->LocationPath.c_str();
        mapData["check_image_path"] = ptr->CheckPath.c_str();
        mapData["ocr_image_path"] = ptr->OCRPath.c_str();
        mapData["location_result"] = ptr->LocationResult.c_str();
        mapData["check_result"] = ptr->CheckResult.c_str();
        mapData["ocr_result"] = ptr->OCRResult.c_str();
        LogInfo("locate check get num: {}", ptr->bottleNum_);
        bool res = PgsqlHelper::getSqlHelper().insertData("circle_product_data", mapData);
        return res;
    }
};