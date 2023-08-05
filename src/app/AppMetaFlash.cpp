#include "AppMetaFlash.h"
#include <QDebug>

QString AppFrame::AppMetaFlash::qmlCallExpected(const ExpectedFunction &functionType, const QString &jsValue)
{
    return appFramework().expected(functionType, jsValue.toStdString()).c_str();
}

void AppFrame::AppMetaFlash::runtimeRoutine(PageIndex itemKey, const QString &value)
{
    static std::unordered_map<PageIndex, std::function<void(const QString &)>> mapSignals_{
        {PageIndex::PageMain, [this](const QString &value) { emit pageMainChange(value); }},
        {PageIndex::PageProduce, [this](const QString &value) { emit pageProduceChange(value); }},
        {PageIndex::PageSensor, [this](const QString &value) { emit pageSensorChange(value); }},
        {PageIndex::PageValve, [this](const QString &value) { emit pageValveChange(value); }},
        {PageIndex::PagePower, [this](const QString &value) { emit pagePowerChange(value); }},
        {PageIndex::PageAlarm, [this](const QString &value) { emit pageAlarmChange(value); }},
        {PageIndex::PageCamera, [this](const QString &value) { emit pageCameraChange(value); }},
        {PageIndex::PageFormula, [this](const QString &value) { emit pageFormulaChange(value); }},
        {PageIndex::PageUser, [this](const QString &value) { emit pageUserChange(value); }}};
    auto signal = mapSignals_.find(itemKey);
    if (signal != mapSignals_.end())
    {
        signal->second(value);
    }
}
