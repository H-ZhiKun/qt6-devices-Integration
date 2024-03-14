#include "AppMetaFlash.h"
#include <QDebug>

AppFrame::AppMetaFlash::AppMetaFlash(QObject *parent) : QObject(parent)
{
    // pageMap初始化
    mapPageName_["main"] = PageIndex::PageMain;
    mapPageName_["produce"] = PageIndex::PageProduce;
    mapPageName_["sensor"] = PageIndex::PageSensor;
    mapPageName_["valve"] = PageIndex::PageValve;
    mapPageName_["power"] = PageIndex::PagePower;
    mapPageName_["alarm"] = PageIndex::PageAlarm;
    mapPageName_["home"] = PageIndex::PageHome;
    mapPageName_["formula"] = PageIndex::PageFormula;
    mapPageName_["user"] = PageIndex::PageUser;
    mapPageName_["log"] = PageIndex::PageLog;
    mapPageName_["debug"] = PageIndex::PageDebug;
    mapPageName_["electric"] = PageIndex::PageElectric;
    mapPageName_["bottom"] = PageIndex::PageBottom;
    mapPageName_["algorithm"] = PageIndex::PageAlgorithm;
    mapPageName_["statistics"] = PageIndex::PageStatistics;
}

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
        {PageIndex::PageHome, [this](const QString &value) { emit pageHomeChange(value); }},
        {PageIndex::PageFormula, [this](const QString &value) { emit pageFormulaChange(value); }},
        {PageIndex::PageUser, [this](const QString &value) { emit pageUserChange(value); }},
        {PageIndex::PageLog, [this](const QString &value) { emit pageLogChange(value); }},
        {PageIndex::PageBottom, [this](const QString &value) { emit pageBottomChange(value); }},
        {PageIndex::PageElectric, [this](const QString &value) { emit pageElectricChange(value); }},
        {PageIndex::PageDebug, [this](const QString &value) { emit pageDebugChange(value); }},
        {PageIndex::PageAlgorithm, [this](const QString &value) { emit pageAlgorithmChange(value); }},
        {PageIndex::PageStatistics, [this](const QString &value) { emit pageStatisticsChange(value); }}};
    auto signal = mapSignals_.find(itemKey);
    if (signal != mapSignals_.end())
    {
        signal->second(value);
    }
}

AppFrame::PageIndex AppFrame::AppMetaFlash::getPageIndex(const std::string &pageName)
{
    auto iter = mapPageName_.find(pageName);
    if (iter != mapPageName_.end())
    {
        return iter->second;
    }
    return PageIndex::PageNull;
}