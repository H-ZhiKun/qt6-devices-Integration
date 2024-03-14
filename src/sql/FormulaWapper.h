#pragma once
#include "PgsqlHelper.h"
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
        jsonVec = PgsqlHelper::getSqlHelper().selectData("formula_data", "", "created_time");

        return jsonVec;
    }

    static QString selectOneFormula(const std::string &name)
    {
        std::string selectStr = "name = '" + name + "'";
        Json::Value value = PgsqlHelper::getSqlHelper().selectData("formula_data", std::move(selectStr), "");
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
        QVariantMap mapData;
        mapData.insert("name", jsonValue["name"].asCString());
        mapData.insert("code_x_position", jsonValue["code_x_position"].asCString());
        mapData.insert("code_y_position", jsonValue["code_y_position"].asCString());
        // mapData.insert("impurity_locate", jsonValue["impurity_locate"].asCString());
        mapData.insert("speed_produce", jsonValue["speed_produce"].asCString());
        mapData.insert("acceleration_produce", jsonValue["acceleration_produce"].asCString());
        mapData.insert("deceleration_produce", jsonValue["deceleration_produce"].asCString());
        mapData.insert("material_diameter", jsonValue["material_diameter"].asCString());
        mapData.insert("bottle_type", jsonValue["bottle_type"].asCString());
        bool res = PgsqlHelper::getSqlHelper().insertData("formula_data", mapData);
        return res;
    }

    static bool modifyFormula(const QString &jsonString)
    {
        QVariantMap mapData;
        Json::Value updateData = Utils::stringToJson(jsonString.toStdString());
        // updateData["code_x_position"] = jsonres["code_x_position"];
        // updateData["code_y_position"] = jsonres["code_y_position"];
        // updateData["impurity_locate"] = jsonres["impurity_locate"];
        // updateData["speed_produce"] = jsonres["speed_produce"];
        // updateData["acceleration_produce"] = jsonres["acceleration_produce"];
        // updateData["deceleration_produce"] = jsonres["deceleration_produce"];
        std::string selectStr = fmt::format("name = '{}'", updateData["name"].asString().c_str());
        updateData.removeMember("name");
        for (auto iter = updateData.begin(); iter != updateData.end(); iter++)
        {
            if (iter.name() == "name")
            {
                continue;
            }
            mapData[iter.name().c_str()] = iter->asCString();
        }
        if (PgsqlHelper::getSqlHelper().updateData("formula_data", mapData, std::move(selectStr)))
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
        if (PgsqlHelper::getSqlHelper().deleteData("formula_data", selectStr))
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

    static bool cancelAutoLoad()
    {
        QVariantMap mapData;
        mapData["auto_load"] = false;
        if (PgsqlHelper::getSqlHelper().updateData("formula_data", mapData, "auto_load = true"))
        {
            return true;
        }
        return false;
    }
};
