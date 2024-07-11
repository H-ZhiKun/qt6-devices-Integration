#include "AppCentralExtension.h"
#include "BaseProduct.h"
#include "CentralClient.h"
#include "ProductWapper.h"
#include <json/value.h>
#include <string>
void AppFrame::AppCentralExtension::uploadData(const std::unordered_map<ReportKey, int32_t> data)
{
    const auto &product = appFramework().getPlugin<BaseProduct>();
    const auto &centralClient = appFramework().getPlugin<CentralClient>();
    if (data.empty() || product == nullptr || centralClient == nullptr)
    {
        return;
    }
    Json::Value jsData;
    // Json::Value jsData = ProductWapper::selectDetailCountData();
    switch (product->getType())
    {
    case TypeProduct::TypeCircle: {
        jsData["device"] = "circle";
        break;
    }
    case TypeProduct::TypeCap: {
        jsData["device"] = "cap";
        break;
    }
    case TypeProduct::TypeImpurity: {
        jsData["device"] = "impurity";
        break;
    }
    case TypeProduct::TypeLine: {
        jsData["device"] = "line";
        break;
    }
    case TypeProduct::TYpeBoxApp: {
        jsData["device"] = "boxApp";
        break;
    }
    default:
        jsData["device"] = "unkonw";
        break;
    }
    for (auto &[key, value] : data)
    {
        jsData[std::to_string(static_cast<int>(key))] = value;
    }
    centralClient->sendProductData(jsData);
    return;
}