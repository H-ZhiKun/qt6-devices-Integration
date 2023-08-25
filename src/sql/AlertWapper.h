#pragma once
#include "PgsqlHelper.h"
#include "Utils.h"
#include "json/json.h"
#include <QDebug>
#include <map>
#include <string>
class AlertWapper
{
#define TABLE_ALARM_DATA "alarm_data"
  public:
    AlertWapper() = default;
    ~AlertWapper() = default;
    static Json::Value selectAlertData()
    {
        Json::Value json;
        std::string condition = "state = " + std::to_string(1);
        json = PgsqlHelper::getSqlHelper().selectData(TABLE_ALARM_DATA, std::move(condition));
        return json;
    }
    static void insertAlert(std::map<std::string, std::string> &mapAlert)
    {
        if (mapAlert.size() == 0)
            return;
        Json::Value json;
        for (auto &[key, value] : mapAlert)
        {
            Json::Value item;
            item["register_address"] = key.c_str();
            item["content"] = value.c_str();
            item["state"] = 1;
            json.append(item);
        }
        PgsqlHelper::getSqlHelper().insertData(TABLE_ALARM_DATA, std::move(json));
    }
    static void modifyAlert(std::map<std::string, std::string> &mapModify)
    {
        if (mapModify.size() == 0)
            return;
        for (auto &[key, value] : mapModify)
        {
            Json::Value item;
            item["state"] = 0;
            std::string condition = fmt::format("`register_address` = '{}'", key);
            PgsqlHelper::getSqlHelper().updateData(TABLE_ALARM_DATA, std::move(item), std::move(condition));
        }
    }

    static void modifyAllStatus()
    {
        Json::Value item;
        item["state"] = 0;
        std::string condition = "state = 1";
        PgsqlHelper::getSqlHelper().updateData(TABLE_ALARM_DATA, std::move(item), std::move(condition));
    }
    static void updateRealtimeAlert(std::map<std::string, std::string> &mapAlert)
    {
        std::map<std::string, std::string> mapCurrent = mapAlert;
        static std::map<std::string, std::string> mapLast;

        // 使用迭代器遍历而不是自增循环
        for (auto lastIter = mapLast.begin(); lastIter != mapLast.end();)
        {
            auto curIter = mapCurrent.find(lastIter->first);
            if (curIter != mapCurrent.end())
            {
                curIter = mapCurrent.erase(curIter);
                lastIter = mapLast.erase(lastIter);
            }
            else
            {
                ++lastIter;
            }
        }

        insertAlert(mapCurrent);
        modifyAlert(mapLast);
        mapLast.swap(mapAlert);
    }

    static int alertNum()
    {
        QString res = PgsqlHelper::getSqlHelper().selectOneData(TABLE_ALARM_DATA, "count(*)");
        return res.toInt();
    }

    static Json::Value selectAlertDataPaged(const int pageSize, const int pageNumber,
                                            const std::string &&condition = "", const std::string &&orderBy = "")
    {
        return PgsqlHelper::getSqlHelper().selectDataPaged(TABLE_ALARM_DATA, pageSize, pageNumber, std::move(condition),
                                                           std::move(orderBy));
    }
};