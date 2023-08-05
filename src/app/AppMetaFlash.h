#pragma once
#include "AppFramework.h"
#include "Logger.h"
#include "Utils.h"
#include "json/json.h"
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
    PageCamera,
    PageFormula,
    PageUser
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
    void pageCameraChange(const QString &value);
    void pageFormulaChange(const QString &value);
    void pageUserChange(const QString &value);

  public:
    static inline AppMetaFlash &instance()
    {
        static AppMetaFlash meta;
        return meta;
    }
    virtual ~AppMetaFlash()
    {
    }
    const char *invokeRuntimeRoutine = "runtimeRoutine";
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
    explicit AppMetaFlash(QObject *parent = nullptr) : QObject(parent)
    {
    }
    // qml (5) Q_PROPERTY 中 属性类型 属性名称 必须一致
    AppMetaFlash(const AppMetaFlash &) = delete;
    AppMetaFlash &operator=(const AppMetaFlash &) = delete;
    AppMetaFlash(AppMetaFlash &&) noexcept(true) = default;
    AppMetaFlash &operator=(AppMetaFlash &&) noexcept(true) = default;
};
} // namespace AppFrame