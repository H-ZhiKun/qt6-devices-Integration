#pragma once
#include "SqlHelper.h"
#include "Utils.h"
#include "json/json.h"
#include <Vector>
class CameraWapper
{
#define TABLE_NAME "camera_config"
  public:
    CameraWapper() = default;
    ~CameraWapper() = default;
    static Json::Value selectAllCamera()
    {
        return SqlHelper::getSqlHelper().selectData(TABLE_NAME, "");
    }
    static QString selectOneCamera(const std::string &name)
    {
        QString res;
        std::string selectStr = "sn_num = '" + name + "'";
        Json::Value value = SqlHelper::getSqlHelper().selectData(TABLE_NAME, std::move(selectStr));
        if (value.size() > 0)
        {
            res = Utils::jsonToString(value[0]).c_str();
        }
        return res;
    }

    static bool modifyCamera(const Json::Value &jsonData)
    {
        bool res = false;
        if (SqlHelper::getSqlHelper().upsertData(TABLE_NAME, std::move(jsonData)))
        {
            LogInfo("Data updated successfully");
            res = true;
        }
        else
        {
            LogError("Failed to update data");
        }
        return res;
    }

    static bool isCameraInDB(const QString &sn_num)
    {
        bool res = SqlHelper::getSqlHelper().checkRecordExist(TABLE_NAME, "sn_num", sn_num.toStdString());
        return res;
    }
};