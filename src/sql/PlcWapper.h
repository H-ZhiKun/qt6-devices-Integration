#pragma once
#include "PgsqlHelper.h"
#include "Utils.h"
#include "json/json.h"

class PlcWapper
{
  public:
    PlcWapper() = default;
    ~PlcWapper() = default;
    static bool modifyPLC(const Json::Value &jsonData)
    {
        bool res = false;
        if (PgsqlHelper::getSqlHelper().upsertData("plc_register_rw", std::move(jsonData)))
        {
            LogInfo("PLCData updated successfully");
            res = true;
        }
        else
        {
            LogError("Failed to update PLCdata");
        }
        return res;
    }

    static bool isPLCInDB(const QString &addr)
    {
        bool res = PgsqlHelper::getSqlHelper().checkRecordExist(plc_register_rw, "plc_address", addr.toStdString());
        return res;
    }
};
