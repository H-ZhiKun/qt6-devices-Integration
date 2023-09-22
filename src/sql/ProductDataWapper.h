#pragma once
#include "BaseProduct.h"
#include "PgsqlHelper.h"
#include "Utils.h"
#include "json/json.h"
#include <QDebug>
#include <map>
#include <memory>
#include <string>

class ProductDataWapper
{
#define TABLE_PRODUCT_DATA "product_data"
  public:
    ProductDataWapper() = default;
    ~ProductDataWapper() = default;
    static bool insert(std::shared_ptr<ProductItem> ptr)
    {
        Json::Value jsonValue;
        TypeProduct type = ptr->typePd_;
        jsonValue["typePd_"] = static_cast<uint8_t>(type);
        jsonValue["bottle_num"] = ptr->bottleNum_;
        jsonValue["batch_num"] = ptr->batchNum_;
        jsonValue["formula_name"] = ptr->formulaName_;
        jsonValue["is_complete"] = ptr->isComplete_;
        jsonValue["qrcode_result"] = ptr->QRCode;
        jsonValue["logistics_true_value_1"] = ptr->logistics1;
        jsonValue["logistics_true_value_2"] = ptr->logistics2;
        jsonValue["location_image_path"] = ptr->LocationPath;
        jsonValue["check_image_path"] = ptr->CheckPath;
        jsonValue["ocr_image_path"] = ptr->OCRPath;
        jsonValue["location_result"] = ptr->LocationResult;
        jsonValue["check_result"] = ptr->CheckResult;
        jsonValue["ocr_result"] = ptr->OCRResult;
        bool res = PgsqlHelper::getSqlHelper().insertData(TABLE_PRODUCT_DATA, std::move(jsonValue));
        return res;
    }
};