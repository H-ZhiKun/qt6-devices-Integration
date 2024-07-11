#pragma once
#include "AppFramework.h"
#include <QCoreApplication>
#include <QObject>
#include <QString>
#include <QVariant>

namespace AppFrame
{
enum class PageIndex
{
    PageMain = 0,
    PageProduce,
    PageSensor,
    PageValve,
    PagePower,
    PageAlarm,
    PageHome,
    PageFormula,
    PageUser,
    PageLog,
    PageDebug,
    PageElectric,
    PageBottom,
    PageAlgorithm,
    PageStatistics,
    PageNull = 100
};
class AppMetaFlash : public QObject
{
    Q_OBJECT
  signals:
    // qml 中响应
    void pageMainChange(const QString &value);
    void pageProduceChange(const QString &value);
    void pageSensorChange(const QString &value);
    void pageValveChange(const QString &value);
    void pagePowerChange(const QString &value);
    void pageAlarmChange(const QString &value);
    void pageHomeChange(const QString &value);
    void pageFormulaChange(const QString &value);
    void pageUserChange(const QString &value);
    void pageLogChange(const QString &value);
    void pageDebugChange(const QString &value);
    void pageBottomChange(const QString &value);
    void pageElectricChange(const QString &value);
    void pageAlgorithmChange(const QString &value);
    void pageStatisticsChange(const QString &value);

  public:
    explicit AppMetaFlash(QObject *parent = nullptr);
    virtual ~AppMetaFlash()
    {
    }
    virtual PageIndex getPageIndex(const std::string &) const;
    std::string getWindName(const std::string &) const;
  public slots:
    // qml 调用c++接口区域
    QString qmlCallExpected(const ExpectedFunction &functionType, const QString &jsValue);
    // cpp 调用qml接口区域

    /**
     * @brief 将JSON格式的字符串转换为Json::Value对象
     * @param itemName 为每个页面取的title，在本类成员中已经定义
     * @param itemName 对应页面实时数据的json字符串
     * @return 返回转换后的Json::Value对象
     */
    void runtimeRoutine(PageIndex itemKey, const QString &itemValue);

  private:
    // qml (5) Q_PROPERTY 中 属性类型 属性名称 必须一致
    AppMetaFlash(const AppMetaFlash &) = delete;
    AppMetaFlash &operator=(const AppMetaFlash &) = delete;
    std::unordered_map<std::string, PageIndex> mapPageName_;
    std::unordered_map<std::string, std::string> mapImageWind_;
};
} // namespace AppFrame