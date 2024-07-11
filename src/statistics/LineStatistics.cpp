#pragma once
#include "LineStatistics.h"
#include <QVariantMap>
#include <fmt/core.h>

LineStatistics::LineStatistics()
{
    type_ = StatisticsType::LineType;
}

QVariantMap LineStatistics::serialization(const std::unordered_map<StatisticsKey, int32_t> &tempMap)
{
    QVariantMap mapData;
    mapData.insert("bottom_num", QString::number(tempMap.at(StatisticsKey::amount)));
    mapData.insert("qrcode_data",
                   QString::fromStdString(fmt::format(
                       "{}/{}/{}/{}", tempMap.at(StatisticsKey::code_trigger), tempMap.at(StatisticsKey::code_get),
                       tempMap.at(StatisticsKey::code_right), tempMap.at(StatisticsKey::code_error))));
    mapData.insert("logistics_data", QString::fromStdString(fmt::format(
                                         "{}/{}/{}", tempMap.at(StatisticsKey::log_feedback),
                                         tempMap.at(StatisticsKey::log_error), tempMap.at(StatisticsKey::log_repeat))));
    mapData.insert("ocr_data", QString::fromStdString(fmt::format("{}/{}/{}/{}", tempMap.at(StatisticsKey::ocr_trigger),
                                                                  tempMap.at(StatisticsKey::ocr_get),
                                                                  tempMap.at(StatisticsKey::ocr_right),
                                                                  tempMap.at(StatisticsKey::ocr_error))));
    mapData.insert("remove_data", QString::fromStdString(fmt::format("{}/{}/{}", tempMap.at(StatisticsKey::normal),
                                                                     tempMap.at(StatisticsKey::remove),
                                                                     tempMap.at(StatisticsKey::all_ng))));

    mapData.insert("print_data", QString::fromStdString(fmt::format("{}/{}", tempMap.at(StatisticsKey::print_send),
                                                                    tempMap.at(StatisticsKey::print_right))));
    return mapData;
}
