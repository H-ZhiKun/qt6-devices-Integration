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
    static QString selectOneCamera(const QString &name)
    {
        QString res;
        QString selectStr = "sn_num = '" + name + "'";
        Json::Value value = SqlHelper::getSqlHelper().selectData(TABLE_NAME, selectStr);
        if (value.size() > 0)
        {
            res = Utils::jsonToString(value[0]).c_str();
        }
        return res;
    }

    static bool modifyCamera(const QString &jsonString)
    {
        bool res = false;
        QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonString.toUtf8());
        QVariantMap updateData = jsonDocument.toVariant().toMap();
        qDebug() << updateData;
        if (SqlHelper::getSqlHelper().upsertData(TABLE_NAME, updateData))
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
        bool res = SqlHelper::getSqlHelper().checkRecordExist(TABLE_NAME, "sn_num", sn_num);
        return res;
    }
};