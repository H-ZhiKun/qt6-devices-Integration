#pragma once
#include "PgsqlHelper.h"
#include "Utils.h"
class UserWapper
{
#define TABLE_USER_NAME "users"
    // #define KEY "kungegeinijiami"
    // #define IV "danzongchusihua"
  public:
    UserWapper() = default;
    ~UserWapper() = default;

    static bool insertUser(const QString &jsonString)
    {
        Json::Value insertData = Utils::stringToJson(jsonString.toStdString());
        //   获取 "password" 键对应的值，并进行加密
        std::string password = insertData["password"].asString();
        password = Utils::encryptByAES(password);
        //  将加密后的密码转换为 QString，并更新 QVariantMap 对象
        QVariantMap mapData;
        mapData.insert("username", insertData["name"].asCString());
        mapData.insert("password", password.c_str());
        mapData.insert("camera_permission", insertData["camera_permission"].asBool());
        mapData.insert("data_permission", insertData["data_permission"].asBool());
        mapData.insert("alarm_permission", insertData["alarm_permission"].asBool());
        mapData.insert("formula_permission", insertData["formula_permission"].asBool());
        mapData.insert("debug_permission", insertData["debug_permission"].asBool());
        mapData.insert("log_permission", insertData["log_permission"].asBool());
        mapData.insert("user_manage_permission", insertData["user_manage_permission"].asBool());
        mapData.insert("auto_login", insertData["auto_login"].asBool());
        bool res = PgsqlHelper::getSqlHelper().insertData("users", mapData);
        if (!res)
        {
            LogError("Failed to insert user data");
        }
        return res;
    }

    static Json::Value selectAllUser()
    {
        Json::Value jsonVec;
        jsonVec = PgsqlHelper::getSqlHelper().selectData(TABLE_USER_NAME, "", "created_time");

        return jsonVec;
    }

    static QString selectUserID(const QString &jsonString)
    {
        QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonString.toUtf8());
        QVariantMap updateData = jsonDocument.toVariant().toMap();
        std::string selectStr = "username = '" + updateData["name"].toString().toStdString() + "'";
        QString userID = PgsqlHelper::getSqlHelper().selectOneData(TABLE_USER_NAME, "id", selectStr, "created_time");
        return userID;
    }

    static bool modifyUser(const QString &jsonString)
    {
        QVariantMap mapData;
        Json::Value updateData = Utils::stringToJson(jsonString.toStdString());
        std::string selectStr = fmt::format("username = '{}'", updateData["name"].asString().c_str());
        updateData.removeMember("name");
        for (auto iter = updateData.begin(); iter != updateData.end(); iter++)
        {
            if (iter.name() == "name" || iter.name() == "password")
            {
                continue;
            }
            LogInfo("modify user: {}", iter.name().c_str());
            LogInfo("modify user key: {}", iter->asBool());
            mapData[iter.name().c_str()] = iter->asBool();
        }
        if (updateData.isMember("password"))
        {
            std::string password = Utils::encryptByAES(updateData["password"].asString());
            mapData["password"] = password.c_str();
            mapData["auto_login"] = false;
        }
        if (PgsqlHelper::getSqlHelper().updateData(TABLE_USER_NAME, mapData, std::move(selectStr)))
        {
            return true;
        }
        else
        {
            LogError("Failed to update user data");
        }
        return false;
    }

    static bool deleteUser(const QString &id)
    {
        std::string selectStr = "id = '" + id.toStdString() + "'";
        if (PgsqlHelper::getSqlHelper().deleteData(TABLE_USER_NAME, selectStr))
        {
            qDebug() << "Data deleted successfully";
            return true;
        }
        else
        {
            qDebug() << "Failed to delete data";
        }
        return false;
    }

    static QString selectUser(const std::string &name)
    {
        std::string selectStr = "username = '" + name + "'";
        Json::Value value = PgsqlHelper::getSqlHelper().selectData(TABLE_USER_NAME, std::move(selectStr), "");
        Json::Value jsonSingleValue;
        for (const Json::Value &jsonValue : value)
        {
            jsonSingleValue = jsonValue;
        }
        std::string password = jsonSingleValue["password"].asString();
        password = Utils::decryptByAES(password);
        jsonSingleValue["password"] = password;
        return Utils::jsonToString(jsonSingleValue).c_str();
    }

    static Json::Value selectLoginUser()
    {
        Json::Value value = PgsqlHelper::getSqlHelper().selectData(TABLE_USER_NAME, "auto_login = true", "");
        Json::Value jsonSingleValue;
        if (value.empty())
        {
            return Json::Value();
        }
        for (const Json::Value &jsonValue : value)
        {
            jsonSingleValue = jsonValue;
        }
        std::string password = jsonSingleValue["password"].asString();
        password = Utils::decryptByAES(password);
        jsonSingleValue["password"] = password;
        return jsonSingleValue;
    }

    static bool cancelAutoLogin()
    {
        QVariantMap mapData;
        mapData["auto_login"] = false;
        if (PgsqlHelper::getSqlHelper().updateData(TABLE_USER_NAME, mapData, "auto_login = true"))
            ;
        {
            return true;
        }
        return false;
    }
};