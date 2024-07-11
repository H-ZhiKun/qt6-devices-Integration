#include "BaseStatistics.h"
#include "StatisticsWapper.h"
#include <chrono>
#include <mutex>
using namespace std::chrono;
BaseStatistics::BaseStatistics(QObject *parent) : QObject(parent)
{
    recordData_ = {
        {StatisticsKey::amount, 0},         {StatisticsKey::code_trigger, 0},  {StatisticsKey::code_get, 0},
        {StatisticsKey::code_right, 0},     {StatisticsKey::code_error, 0},    {StatisticsKey::log_feedback, 0},
        {StatisticsKey::log_error, 0},      {StatisticsKey::log_repeat, 0},    {StatisticsKey::print_send, 0},
        {StatisticsKey::print_right, 0},    {StatisticsKey::ocr_trigger, 0},   {StatisticsKey::ocr_get, 0},
        {StatisticsKey::ocr_right, 0},      {StatisticsKey::ocr_error, 0},     {StatisticsKey::locate_trigger, 0},
        {StatisticsKey::locate_get, 0},     {StatisticsKey::locate_right, 0},  {StatisticsKey::locate_error, 0},
        {StatisticsKey::check_trigger, 0},  {StatisticsKey::check_get, 0},     {StatisticsKey::check_right, 0},
        {StatisticsKey::check_error, 0},    {StatisticsKey::check_perfect, 0}, {StatisticsKey::check_good, 0},
        {StatisticsKey::check_commonly, 0}, {StatisticsKey::check_bad, 0},     {StatisticsKey::remove, 0},
        {StatisticsKey::all_ng, 0},         {StatisticsKey::normal, 0}};
    reportData_ = {{ReportKey::amount, 0},        {ReportKey::faile, 0},         {ReportKey::qrcodeFaile, 0},
                   {ReportKey::exceptionCode, 0}, {ReportKey::duplicateCode, 0}, {ReportKey::blackFaile, 0}};
}
bool BaseStatistics::isContinuousNotGood()
{
    bool ret = false;
    if (ngCount_ >= ngMax_)
    {
        ret = true;
        LogInfo("consitant {} ng", ngCount_.load());
        ngCount_ = 0;
    }
    return ret;
}
void BaseStatistics::updateCount(const StatisticsKey key)
{
    std::lock_guard lock(mtxCount_);
    recordData_[key]++;
    if (key == StatisticsKey::amount)
    { // 计算时间间隔
        time_point<system_clock> updateTime = system_clock::now();
        auto duration = updateTime - dataClock_;
        if (duration >= hours(1))
        {
            dataClock_ = updateTime;
            int splitCount = recordData_.at(StatisticsKey::remove) + recordData_.at(StatisticsKey::normal);
            std::unordered_map<StatisticsKey, int32_t> splitData;
            for (auto &[key, value] : recordData_)
            {
                auto tempSplit = value - splitCount;
                if (tempSplit > 0)
                {
                    splitData[key] = splitCount;
                    value = tempSplit;
                }
                else
                {
                    splitData[key] = value;
                    value = 0;
                }
            }
            StatisticsWapper::updateStatistics(serialization(splitData));
            StatisticsWapper::insertStatistics(serialization(recordData_));
        }
    }
    else
    {
        if (key == StatisticsKey::remove)
        {
            ngCount_++;
            LogInfo("ngCount_: {}", ngCount_.load());
        }
        if (key == StatisticsKey::normal)
        {
            ngCount_ = 0;
        }
        StatisticsWapper::updateStatistics(serialization(recordData_));
    }
}

void BaseStatistics::setNgMax(uint32_t ngNum)
{
    ngMax_ = ngNum;
    return;
}

void BaseStatistics::updateReport(const std::vector<ReportKey> keys)
{
    std::lock_guard lock(mtxReport_);
    for (auto it : keys)
    {
        reportData_[it]++;
    }
    // 计算时间间隔
    time_point<system_clock> updateTime = system_clock::now();
    auto duration = updateTime - reportClock_;
    if (duration >= seconds(5))
    {
        reportClock_ = updateTime;
        emit sendReport(reportData_);
        reportReset();
    }
}

void BaseStatistics::reportReset()
{
    for (auto &[key, value] : reportData_)
    {
        value = 0;
    }
}

void BaseStatistics::sendReportEmit()
{
    std::lock_guard lock(mtxReport_);
    emit sendReport(reportData_);
}

BaseStatistics::~BaseStatistics()
{
    // std::lock_guard lock(mtxReport_);
    // emit sendReport(reportData_);
}