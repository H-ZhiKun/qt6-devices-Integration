#pragma once
#include "PgsqlHelper.h"
#include "Utils.h"
#include "json/json.h"
#include <QDebug>
#include <map>
#include <string>
class AlgorithmWapper
{
#define TABLE_ALGORITHM_DATA "algorithm_config"
  public:
    AlgorithmWapper() = default;
    ~AlgorithmWapper() = default;
    static Json::Value selectAlgorithmData()
    {
        Json::Value json;
        json = PgsqlHelper::getSqlHelper().selectData(TABLE_ALGORITHM_DATA, "", "created_time");
        return json;
    }
    static void insertAlgorithm(std::map<std::string, std::string> &mapAlgorithm)
    {
        if (mapAlgorithm.size() == 0)
            return;
        QList<QVariantMap> sqlList;
        for (auto &[key, value] : mapAlgorithm)
        {
            QVariantMap mapData;
            mapData.insert(key.c_str(), value.c_str());
            sqlList.append(mapData);
        }
        PgsqlHelper::getSqlHelper().insertMultipleData(TABLE_ALGORITHM_DATA, sqlList);
    }
    static bool modifyAlgorithm(const QString &jsonString)
    {
        QVariantMap mapData;
        Json::Value updateData = Utils::stringToJson(jsonString.toStdString());
        std::string selectStr = "id = '1'";
        for (auto iter = updateData.begin(); iter != updateData.end(); iter++)
        {
            if (iter.name() == "name")
            {
                continue;
            }
            mapData[iter.name().c_str()] = iter->asCString();
        }
        if (PgsqlHelper::getSqlHelper().updateData(TABLE_ALGORITHM_DATA, mapData, std::move(selectStr)))
        {
            qDebug() << "Data updated successfully";
            return true;
        }
        else
        {
            qDebug() << "Failed to update data";
        }
        return false;
    }
};