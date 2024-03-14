#pragma once
#include "PgsqlHelper.h"
#include <json/json.h>
#include <string>

class ElecWapper
{
  public:
    ElecWapper() = default;
    ~ElecWapper() = default;
    static bool insertData(const Json::Value &jsonData)
    {
        QVariantMap mapData;
        if (jsonData.isMember("a_phase_voltage"))
        {
            mapData.insert("a_phase_voltage", QString().number(jsonData["a_phase_voltage"].asDouble(), 'f', 2));
            mapData.insert("b_phase_voltage", QString().number(jsonData["b_phase_voltage"].asDouble(), 'f', 2));
            mapData.insert("c_phase_voltage", QString().number(jsonData["c_phase_voltage"].asDouble(), 'f', 2));
            mapData.insert("a_direction_current", QString().number(jsonData["a_direction_current"].asDouble(), 'f', 2));
            mapData.insert("b_direction_current", QString().number(jsonData["b_direction_current"].asDouble(), 'f', 2));
            mapData.insert("c_direction_current", QString().number(jsonData["c_direction_current"].asDouble(), 'f', 2));
            mapData.insert("total_active_power", QString().number(jsonData["total_active_power"].asDouble(), 'f', 2));
            mapData.insert("total_apparent_power",
                           QString().number(jsonData["total_apparent_power"].asDouble(), 'f', 2));
            mapData.insert("temperature", QString().number(jsonData["temperature"].asDouble(), 'f', 2));
            mapData.insert("humidity", QString().number(jsonData["humidity"].asDouble(), 'f', 2));
        }
        if (jsonData.isMember("total_active_energy"))
        {
            mapData.insert("total_active_energy", QString().number(jsonData["total_active_energy"].asDouble(), 'f', 2));
        }
        if (jsonData.isMember("combined_active_energy"))
        {
            mapData.insert("combined_active_energy",
                           QString().number(jsonData["combined_active_energy"].asDouble(), 'f', 2));
            mapData.insert("positive_active_energy",
                           QString().number(jsonData["positive_active_energy"].asDouble(), 'f', 2));
            mapData.insert("reverse_active_energy",
                           QString().number(jsonData["reverse_active_energy"].asDouble(), 'f', 2));
        }
        bool res = PgsqlHelper::getSqlHelper().insertData("electric_data", mapData);
        if (!res)
        {
            LogError("Failed to insert elec data");
        }
        return res;
    }
};