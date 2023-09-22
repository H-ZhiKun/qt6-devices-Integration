#pragma once
#include "BaseProduct.h"
#include "PgsqlHelper.h"
#include "Utils.h"
#include "json/json.h"
#include <QDebug>
#include <map>
#include <memory>
#include <string>

class ProductTimeWapper
{
#define TABLE_PRODUCT_TIME "product_time"
  public:
    ProductTimeWapper() = default;
    ~ProductTimeWapper() = default;
    static bool insert(std::shared_ptr<ProductItem> ptr)
    {
        Json::Value jsonValue;
        TypeProduct type = ptr->typePd_;
        jsonValue["typePd_"] = static_cast<uint8_t>(type);
        jsonValue["bottle_num"] = ptr->bottleNum_;
        jsonValue["batch_num"] = ptr->batchNum_;
        jsonValue["formula_name"] = ptr->formulaName_;
        jsonValue["is_complete"] = ptr->isComplete_;
        jsonValue["qrcode_signal_time"] = ptr->QRSigTime;
        jsonValue["qrcode_time"] = ptr->QRCodeTime;
        jsonValue["logistics_ret_time"] = ptr->logisticsTime;
        jsonValue["location_signal_time"] = ptr->LocationSigTime;
        jsonValue["location_image_time"] = ptr->LocationImageTime;
        jsonValue["location_result_time"] = ptr->LocationResultTime;
        jsonValue["check_signal_time"] = ptr->CheckSigTime;
        jsonValue["check_image_time"] = ptr->CheckImageTime;
        jsonValue["check_result_time"] = ptr->CheckResultTime;
        jsonValue["coding_signal_time"] = ptr->codingSigTime;
        jsonValue["ocr_signal_time"] = ptr->OCRSigTime;
        jsonValue["ocr_image_time"] = ptr->OCRImageTime;
        jsonValue["ocr_result_time"] = ptr->OCRResultTime;
        jsonValue["complete_signal_time"] = ptr->completeSigTime;
        // Json::Value jsonValue = Utils::stringToJson(jsonString.toStdString());
        bool res = PgsqlHelper::getSqlHelper().insertData(TABLE_PRODUCT_TIME, std::move(jsonValue));
        return res;
    }
};