#pragma once
#include "PgsqlHelper.h"
#include "Utils.h"
#include "json/json.h"
#include <QDebug>
#include <map>
#include <string>
class ProductWapper
{
#define TABLE_PRODUCT_TIME "product_time"
  public:
    ProductWapper() = default;
    ~ProductWapper() = default;
    static bool insert(const QString &jsonString)
    {
        Json::Value jsonValue = Utils::stringToJson(jsonString.toStdString());
        bool res = PgsqlHelper::getSqlHelper().insertData(TABLE_PRODUCT_TIME, std::move(jsonValue));
        return res;
    }
};