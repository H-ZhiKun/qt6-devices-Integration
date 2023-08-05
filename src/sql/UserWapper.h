#pragma once
#include "SqlHelper.h"
#include "Utils.h"
class UserWapper
{
#define TABLE_USER_NAME "user"
    // #define KEY "kungegeinijiami"
    // #define IV "danzongchusihua"
  public:
    UserWapper() = default;
    ~UserWapper() = default;

    static bool insertUser(const QString &jsonString)
    {
        QList<QVariantMap> dataList;
        QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonString.toUtf8());
        //   获取 "password" 键对应的值，并进行加密
        std::string password = jsonDocument["password"].toString().toStdString();
        password = Utils::encrytByAES(password);
        //  将加密后的密码转换为 QString，并更新 QVariantMap 对象
        QVariantMap insertData = jsonDocument.toVariant().toMap();
        insertData["password"] = QString::fromStdString(password);

        dataList.append(insertData);
        bool res = SqlHelper::getSqlHelper().insertData(TABLE_USER_NAME, dataList);
        if (!res)
        {
            LogError("Failed to insert user data");
        }
        return res;
    }

    static Json::Value selectAllUser()
    {
        Json::Value jsonVec;
        jsonVec = SqlHelper::getSqlHelper().selectData(TABLE_USER_NAME, "", "created_time");

        return jsonVec;
    }

    static QString selectUserID(const QString &jsonString)
    {
        QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonString.toUtf8());
        QVariantMap updateData = jsonDocument.toVariant().toMap();
        QString selectStr = "name = '" + updateData["name"].toString() + "'";
        QString userID = SqlHelper::getSqlHelper().selectOneData(TABLE_USER_NAME, "id", selectStr, "created_time");
        return userID;
    }

    static bool modifyUser(const QString &jsonString)
    {
        QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonString.toUtf8());
        QVariantMap updateData = jsonDocument.toVariant().toMap();
        if (updateData.contains("password"))
        {
            QString qPassword = updateData["password"].toString();
            std::string password = Utils::encrytByAES(qPassword.toStdString());
            updateData["password"] = QString::fromStdString(password);
        }
        QString selectStr = "name = '" + updateData["name"].toString() + "'";
        updateData.remove("name");
        if (SqlHelper::getSqlHelper().updateData(TABLE_USER_NAME, updateData, selectStr))
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
        QString selectStr = "id = '" + id + "'";
        if (SqlHelper::getSqlHelper().deleteData(TABLE_USER_NAME, selectStr))
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

    static QString selectUser(const QString &name)
    {
        QString selectStr = "name = '" + name + "'";
        Json::Value value = SqlHelper::getSqlHelper().selectData(TABLE_USER_NAME, selectStr, "");
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