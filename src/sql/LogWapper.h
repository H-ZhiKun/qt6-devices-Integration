#pragma once
#include "PgsqlHelper.h"
#include "Utils.h"
#include <QDebug>
#include <json/json.h>
#include <string>


class LogWapper
{
#define TABLE_LOG_DATA "operate_log"
  public:
    LogWapper() = default;
    ~LogWapper() = default;

    // 根据页数获取操作记录
    static Json::Value selectLogDataPaged(const int pageSize, const int pageNumber, const std::string &&condition = "",
                                          const std::string &&orderBy = "")
    {
        return PgsqlHelper::getSqlHelper().selectDataPaged(TABLE_LOG_DATA, pageSize, pageNumber, std::move(condition),
                                                           std::move(orderBy));
    }

    // 添加记录
    static bool insertLog(const std::string &jsonString)
    {
        Json::Value insertData = Utils::stringToJson(jsonString);
        QVariantMap mapData;
        mapData.insert("operate_name", insertData["operate_name"].asCString());
        mapData.insert("operate_page", insertData["operate_page"].asCString());
        mapData.insert("operate_function", insertData["operate_function"].asCString());
        mapData.insert("detail", insertData["detail"].asCString());

        bool res = PgsqlHelper::getSqlHelper().insertData(TABLE_LOG_DATA, mapData);
        if (!res)
        {
            LogError("Failed to insert operate_log data");
        }
        return res;
    }

    static int LogNum()
    {
        QString res = PgsqlHelper::getSqlHelper().selectOneData(TABLE_LOG_DATA, "count(*)");
        return res.toInt();
    }
};