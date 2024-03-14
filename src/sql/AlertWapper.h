#pragma once
#include "PgsqlHelper.h"
#include "Utils.h"
#include <QDebug>
#include <json/json.h>
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
        QList<QVariantMap> sqlList;
        for (auto &[key, value] : mapAlert)
        {
            QVariantMap mapData;
            mapData.insert("register_address", key.c_str());
            mapData.insert("content", value.c_str());
            mapData.insert("state", true);
            sqlList.append(mapData);
        }
        PgsqlHelper::getSqlHelper().insertMultipleData("alarm_data", sqlList);
    }
    static void modifyAlert(std::map<std::string, std::string> &mapModify)
    {
        if (mapModify.size() == 0)
            return;
        QVariantMap mapData;
        for (auto &[key, value] : mapModify)
        {
            mapData["state"] = false;
            std::string condition = fmt::format("register_address = '{}' and state = true", key);
            PgsqlHelper::getSqlHelper().updateData(TABLE_ALARM_DATA, mapData, std::move(condition));
        }
    }

    static void modifyAllStatus()
    {
        QVariantMap mapData;
        mapData["state"] = false;
        std::string condition = "state = TRUE";
        PgsqlHelper::getSqlHelper().updateData(TABLE_ALARM_DATA, mapData, std::move(condition));
    }
    static void updateRealtimeAlert(std::map<std::string, std::string> &current,
                                    std::map<std::string, std::string> &record)
    {
        if (current.empty() && record.empty())
        {
            return;
        }
        auto temp = current;
        auto recIter = record.begin();
        while (recIter != record.end())
        {
            auto curIter = current.find(recIter->first);
            if (curIter != current.end())
            {
                curIter = current.erase(curIter);
                recIter = record.erase(recIter);
            }
            else
            {
                ++recIter;
            }
        }
        insertAlert(current);
        modifyAlert(record);
        record = temp;
        current.clear();
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

    static std::string selectAlarmingData()
    {
        Json::Value jsval =
            PgsqlHelper::getSqlHelper().selectData(TABLE_ALARM_DATA, "state = true", "created_time desc");
        std::string str = "";
        if (jsval.empty())
        {
            return str;
        }
        else
        {
            int i = 0;
            for (Json::Value &js : jsval)
            {
                i++;
                str += Utils::jsonToString(js["content"]);
                str += "\n";
                if (i == 3)
                {
                    break;
                }
            }
        }
        return str;
    }
};