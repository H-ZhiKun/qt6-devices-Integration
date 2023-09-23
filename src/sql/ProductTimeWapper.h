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
        std::string sql = fmt::format(
            "INSERT INTO product_time (typePd_, bottle_num, batch_num, formula_name, is_complete,\
        qrcode_signal_time, qrcode_time, \
        logistics_ret_time, location_signal_time, \
        location_image_time, location_result_time,\
         check_signal_time, check_image_time, \
         check_result_time, coding_signal_time,\
         ocr_signal_time, ocr_image_time, \
         ocr_result_time, complete_signal_time) VALUES (\
           {}, '{}', '{}', '{}', {}, \
           '{}', '{}', \
           '{}', '{}', \
           '{}', '{}', \
           '{}', '{}', \
           '{}', '{}', \
           '{}', '{}', \
           '{}', '{}';",
            static_cast<uint8_t>(ptr->typePd_), ptr->bottleNum_, ptr->batchNum_, ptr->formulaName_, ptr->isComplete_,
            ptr->QRSigTime, ptr->QRCodeTime, ptr->logisticsTime, ptr->LocationSigTime, ptr->LocationImageTime,
            ptr->LocationResultTime, ptr->CheckSigTime, ptr->CheckImageTime, ptr->CheckResultTime, ptr->codingSigTime,
            ptr->OCRSigTime, ptr->OCRImageTime, ptr->OCRResultTime, ptr->completeSigTime);

        bool res = PgsqlHelper::getSqlHelper().insertData(std::move(sql));
        return res;
    }
};