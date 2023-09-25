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
        password = Utils::encrytByAES(password);
        //  将加密后的密码转换为 QString，并更新 QVariantMap 对象

        insertData["password"] = password;

        std::string sql = fmt::format(
            "INSERT INTO {} ('name', 'password', 'camera_permission', 'data_permission', 'alarm_permission', \
            'formula_permission',\
            'sensor_permission', 'valve_permission', 'power_permission', 'log_permission', \
            'user_manage_permission') VALUES('{}', '{}', '{}', '{}', '{}', '{}', '{}', '{}', '{}', '{}', '{}');",
            "users", insertData["name"].asCString(), insertData["password"].asCString(),
            insertData["camera_permission"].asBool(), insertData["data_permission"].asBool(),
            insertData["alarm_permission"].asBool(), insertData["formula_permission"].asBool(),
            insertData["sensor_permission"].asBool(), insertData["valve_permission"].asBool(),
            insertData["power_permission"].asBool(), insertData["log_permission"].asBool(),
            insertData["user_manage_permission"].asBool());
        bool res = PgsqlHelper::getSqlHelper().insertData(std::move(sql));
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
        std::string selectStr = "name = '" + updateData["name"].toString().toStdString() + "'";
        QString userID = PgsqlHelper::getSqlHelper().selectOneData(TABLE_USER_NAME, "id", selectStr, "created_time");
        return userID;
    }

    static bool modifyUser(const QString &jsonString)
    {
        Json::Value updateData = Utils::stringToJson(jsonString.toStdString());
        if (updateData.isMember("password"))
        {
            std::string password = Utils::encrytByAES(updateData["password"].asString());
            updateData["password"] = password;
        }
        std::string selectStr = fmt::format("name = '{}'", updateData["name"].asString().c_str());
        updateData.removeMember("name");
        if (PgsqlHelper::getSqlHelper().updateData(TABLE_USER_NAME, std::move(updateData), std::move(selectStr)))
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
        std::string selectStr = "name = '" + name + "'";
        Json::Value value = PgsqlHelper::getSqlHelper().selectData(TABLE_USER_NAME, std::move(selectStr), "");
        Json::Value jsonSingleValue;
        for (const Json::Value &jsonValue : value)
        {
            jsonSingleValue = jsonValue;
        }
        std::string password = jsonSingleValue["password"].asString();
        password = Utils::decrytByAES(password);
        jsonSingleValue["password"] = password;
        return Utils::jsonToString(jsonSingleValue).c_str();
    }
};