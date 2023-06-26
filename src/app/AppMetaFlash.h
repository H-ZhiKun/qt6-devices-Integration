#pragma once
#include "Logger.h"
#include "Utils.h"
#include <QCoreApplication>
#include <QObject>
#include <QString>
#include <QVariant>

namespace AppFrame
{
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

  public:
    explicit AppMetaFlash(QObject *parent = nullptr) : QObject(parent)
    {
        mapSignals_[strMain] = [this](const QString &value) { emit pageMainChange(value); };
        mapSignals_[strProduce] = [this](const QString &value) { emit pageMainChange(value); };
        mapSignals_[strSensor] = [this](const QString &value) { emit pageMainChange(value); };
        mapSignals_[strValve] = [this](const QString &value) { emit pageMainChange(value); };
        mapSignals_[strPower] = [this](const QString &value) { emit pageMainChange(value); };
    }
    virtual ~AppMetaFlash()
    {
    }
  public slots:
    // qml 调用c++接口区域
    bool qmlDominoConnect(const QString &ip = "", quint16 port = 0);
    // cpp 调用qml接口区域
    /**
     * @brief 将JSON格式的字符串转换为Json::Value对象
     * @param itemName 为每个页面取的title，在本类成员中已经定义
     * @param itemName 对应页面实时数据的json字符串
     * @return 返回转换后的Json::Value对象
     */
    void setRealTimeValue(const QString &itemName, const QString &itemValue);

    QString invokeSetRealTimeValue()
    {
        return "setRealTimeValue";
    }

  private:
    // qml (5) Q_PROPERTY 中 属性类型 属性名称 必须一致
    std::unordered_map<std::string, std::function<void(const QString &)>> mapSignals_;
    AppMetaFlash(const AppMetaFlash &) = delete;
    AppMetaFlash &operator=(const AppMetaFlash &) = delete;
    AppMetaFlash(AppMetaFlash &&) noexcept(true) = default;
    AppMetaFlash &operator=(AppMetaFlash &&) noexcept(true) = default;

  public:
    const char *strMain = "Main";
    const char *strProduce = "Produce";
    const char *strSensor = "Sensor";
    const char *strValve = "Valve";
    const char *strPower = "Power";
};
} // namespace AppFrame