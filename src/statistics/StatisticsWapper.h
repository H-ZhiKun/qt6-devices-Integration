#pragma once
#include "PgsqlHelper.h"
#include <cstdint>
#include <fmt/core.h>
#include <json/json.h>
#include <string>

class StatisticsWapper
{
#define TABLE_STATISTICS_DATA "statistics"
    inline static uint32_t id_ = 0;

  public:
    StatisticsWapper() = default;
    ~StatisticsWapper() = default;
    static bool insertStatistics(const QVariantMap &mapData)
    {
        bool ret = false;
        ret = PgsqlHelper::getSqlHelper().insertData(TABLE_STATISTICS_DATA, mapData);
        std::string orderSql = "created_time DESC LIMIT 1";
        auto json = PgsqlHelper::getSqlHelper().selectData(TABLE_STATISTICS_DATA, "", std::move(orderSql));
        if (json.isArray() && json[0].isMember("id"))
        {
            id_ = std::stoul(json[0]["id"].asString());
        }
        else
        {
            ret = false;
        }
        return ret;
    }
    static bool updateStatistics(const QVariantMap &mapData)
    {
        std::string update = fmt::format("id = {}", id_);
        return PgsqlHelper::getSqlHelper().updateData(TABLE_STATISTICS_DATA, mapData, std::move(update));
    }
    static Json::Value selectDataPaged(const int pageSize, const int pageNumber, const std::string &&condition = "",
                                       const std::string &&orderBy = "")
    {
        return PgsqlHelper::getSqlHelper().selectDataPaged(TABLE_STATISTICS_DATA, pageSize, pageNumber,
                                                           std::move(condition), std::move(orderBy));
    }
    static int dataCount()
    {
        QString res = PgsqlHelper::getSqlHelper().selectOneData(TABLE_STATISTICS_DATA, "count(*)");
        return res.toInt();
    }
};