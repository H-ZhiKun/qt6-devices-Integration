#pragma once
#include "BaseProduct.h"
#include "PgsqlHelper.h"
#include "Utils.h"
#include "json/json.h"
#include <QDebug>
#include <map>
#include <memory>
#include <string>

class LineProductDataWapper
{
#define TABLE_PRODUCT_DATA "product_data"
  public:
    LineProductDataWapper() = default;
    ~LineProductDataWapper() = default;
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
        mapData["ocr_image_path"] = ptr->OCRPath.c_str();
        mapData["ocr_result"] = ptr->OCRResult.c_str();
        bool res = PgsqlHelper::getSqlHelper().insertData("line_product_data", mapData);
        return res;
    }
};