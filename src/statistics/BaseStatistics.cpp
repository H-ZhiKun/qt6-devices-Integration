#include "BaseStatistics.h"
#include "StatisticsWapper.h"

BaseStatistics::BaseStatistics(QObject *parent) : QObject(parent)
{
}

void BaseStatistics::updateCount(const StatisticsKey key)
{
    recordData_.at(key)++;
    if (key == StatisticsKey::amount)
    { // 计算时间间隔
        time_point<system_clock> updateTime = system_clock::now();
        auto duration = updateTime - dataClock_;
        if (duration >= hours(1))
        {
            dataClock_ = updateTime;
            for (auto &[key, val] : recordData_)
            {
                val = 0;
            }
            recordData_.at(key)++;
            StatisticsWapper::insertStatistics(serialization());
        }
    }
    else
    {
        if (key == StatisticsKey::ocr_trigger && type_ == StatisticsType::CapType)
        {
            recordData_.at(StatisticsKey::code_trigger)++;
        }
        StatisticsWapper::updateStatistics(serialization());
    }
}

QVariantMap BaseStatistics::serialization()
{
    QVariantMap mapData;
    mapData.insert("bottom_num", QString::number(recordData_[StatisticsKey::amount]));
    mapData.insert("qrcode_data",
                   QString::fromStdString(fmt::format(
                       "{}/{}/{}/{}", recordData_[StatisticsKey::code_trigger], recordData_[StatisticsKey::code_get],
                       recordData_[StatisticsKey::code_right], recordData_[StatisticsKey::code_feedback])));
    mapData.insert("print_data", QString::fromStdString(fmt::format("{}/{}", recordData_[StatisticsKey::print_send],
                                                                    recordData_[StatisticsKey::print_right])));
    mapData.insert("ocr_data", QString::fromStdString(fmt::format("{}/{}", recordData_[StatisticsKey::ocr_trigger],
                                                                  recordData_[StatisticsKey::ocr_get])));
    mapData.insert("tangle_data",
                   QString::fromStdString(fmt::format("{}/{}/{}", recordData_[StatisticsKey::locate_trigger],
                                                      recordData_[StatisticsKey::locate_get],
                                                      recordData_[StatisticsKey::locate_right])));
    mapData.insert("check_data", QString::fromStdString(fmt::format(
                                     "{}/{}/{}", recordData_[StatisticsKey::check_trigger],
                                     recordData_[StatisticsKey::check_get], recordData_[StatisticsKey::check_right])));
    mapData.insert("remove_data", QString::number(recordData_[StatisticsKey::remove]));
    return mapData;
}