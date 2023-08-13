#pragma once
#include "SqlHelper.h"
#include "Utils.h"
#include "json/json.h"
#include <Vector>
class FormulaWapper
{
  public:
    FormulaWapper() = default;
    ~FormulaWapper() = default;
    static Json::Value selectAllFormula()
    {
        Json::Value jsonVec;
        jsonVec = SqlHelper::getSqlHelper().selectData("formula_data", "", "created_time");

        return jsonVec;
    }

    static QString selectOneFormula(const std::string &name)
    {
        std::string selectStr = "name = '" + name + "'";
        Json::Value value = SqlHelper::getSqlHelper().selectData("formula_data", std::move(selectStr), "");
        Json::Value jsonSingleValue;
        for (const Json::Value &jsonValue : value)
        {
            jsonSingleValue = jsonValue;
        }
        return Utils::jsonToString(jsonSingleValue).c_str();
    }

    static bool insertFormula(const QString &jsonString)
    {
        Json::Value jsonValue = Utils::stringToJson(jsonString.toStdString());
        bool res = SqlHelper::getSqlHelper().insertData("formula_data", std::move(jsonValue));
        return res;
    }

    static bool modifyFormula(const QString &jsonString)
    {
        Json::Value updateData = Utils::stringToJson(jsonString.toStdString());
        // updateData["code_x_position"] = jsonres["code_x_position"];
        // updateData["code_y_position"] = jsonres["code_y_position"];
        // updateData["impurity_locate"] = jsonres["impurity_locate"];
        // updateData["speed_produce"] = jsonres["speed_produce"];
        // updateData["acceleration_produce"] = jsonres["acceleration_produce"];
        // updateData["deceleration_produce"] = jsonres["deceleration_produce"];
        std::string selectStr = fmt::format("name = '{}'", updateData["name"].asString().c_str());
        updateData.removeMember("name");
        if (SqlHelper::getSqlHelper().updateData("formula_data", std::move(updateData), std::move(selectStr)))
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

    static bool deleteFormula(const QString &name)
    {
        std::string selectStr = "name = '" + name.toStdString() + "'";
        if (SqlHelper::getSqlHelper().deleteData("formula_data", selectStr))
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
};
