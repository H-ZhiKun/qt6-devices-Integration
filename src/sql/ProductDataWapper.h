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

        std::string sql =
            fmt::format("INSERT INTO product_time (typePd_, bottle_num, batch_num, formula_name, is_complete,\
        qrcode_result, logistics_true_value_1, \
        logistics_true_value_2, location_image_path, \
         check_image_path, ocr_image_path, \
         location_result, check_result,\
         ocr_result) VALUES (\
           {}, '{}', '{}', '{}', {}, \
           '{}', '{}', \
           '{}', '{}', \
           '{}', '{}', \
           '{}', '{}', \
           '{}';",
                        static_cast<uint8_t>(ptr->typePd_), ptr->bottleNum_, ptr->batchNum_, ptr->formulaName_,
                        ptr->isComplete_, ptr->QRCode, ptr->logistics1, ptr->logistics2, ptr->LocationPath,
                        ptr->CheckPath, ptr->OCRPath, ptr->LocationResult, ptr->CheckResult, ptr->OCRResult);

        // Json::Value jsonValue;
        // TypeProduct type = ptr->typePd_;
        // jsonValue["typePd_"] = static_cast<uint8_t>(type);
        // jsonValue["bottle_num"] = ptr->bottleNum_;
        // jsonValue["batch_num"] = ptr->batchNum_;
        // jsonValue["formula_name"] = ptr->formulaName_;
        // jsonValue["is_complete"] = ptr->isComplete_;
        // jsonValue["qrcode_result"] = ptr->QRCode;
        // jsonValue["logistics_true_value_1"] = ptr->logistics1;
        // jsonValue["logistics_true_value_2"] = ptr->logistics2;
        // jsonValue["location_image_path"] = ptr->LocationPath;
        // jsonValue["check_image_path"] = ptr->CheckPath;
        // jsonValue["ocr_image_path"] = ptr->OCRPath;
        // jsonValue["location_result"] = ptr->LocationResult;
        // jsonValue["check_result"] = ptr->CheckResult;
        // jsonValue["ocr_result"] = ptr->OCRResult;
        bool res = PgsqlHelper::getSqlHelper().insertData(TABLE_PRODUCT_DATA, std::move(sql));
        return res;
    }
};