#pragma once
#include "NonCopyable.h"
#include <QObject>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <functional>
#include <memory>
#include <qobject.h>
#include <string>

namespace AppFrame
{

enum class ExpectedFunction
{
    InsertSqlData = 0,    // 插入某个数据
    DeleteSqlData = 1,    // 删除某个数据
    SelectSqlData = 2,    // 查询某个配方数据
    ModifySqlData = 3,    // 修改某个配方数据
    ReadPLC = 4,          // 读PLC数据
    WritePLC = 5,         // 写PLC数据
    RefreshMainPage = 6,  // 刷新主页面数据
    RefreshPowerPage = 7, // 刷新电能数据
    ZeroClearing = 8,     // 计数清零
    AutoLogin = 9,        // 自动登录
    ProduceReset = 10,    // 复位
    SelectProduct = 11,   // 刷新瓶子数据
    AutoLoad = 12,        // 自动下载配方
    InitPageData = 13,    // 初始化页面数据
    PowerOff = 14,        // 自动关机
    GetPLCCount = 15,     // 获取PLC计数
    GetDataReport = 16    // 获取测试数据报告
};

class AppFramework : public NonCopyable
{
  public:
    virtual ~AppFramework() = default;
    /**
     * @brief 获取应用程序框架的唯一实例
     * @details 该函数返回应用程序框架的唯一实例，是单例模式的实现。该函数不接收任何参数。
     * @return 返回AppFramework类型的引用，表示应用程序框架的唯一实例。
     */
    static AppFramework &instance();
    virtual int run(QQmlApplicationEngine *) = 0;
    virtual QQmlApplicationEngine *getEngine() = 0;
    virtual std::string expected(const ExpectedFunction &expectedType, const std::string &jsValue) = 0;
    virtual bool registerExpectation(const ExpectedFunction &expectedType,
                                     std::function<std::string(const std::string &)> &&api) = 0;
    virtual void quitProgram() = 0;

  public:
    // 注册插件
    template <typename InputType> void registerPlugin(std::unique_ptr<QObject> &&obj)
    {
        plugins_[InputType::staticMetaObject.className()] = std::move(obj);
    }

    // 获取插件
    template <typename NeedType> NeedType *getPlugin()
    {
        const auto &name = NeedType::staticMetaObject.className();
        auto it = plugins_.find(name);
        if (it != plugins_.end())
        {
            return qobject_cast<NeedType *>(it->second.get());
        }
        else
        {
            return nullptr;
        }
    }

    // 调用C++ 对象函数工具
    template <typename... Args> bool invokeCpp(QObject *object, const char *functionName, Args &&...arguments)
    {
        bool success = false;
        if (object)
        {
            // QVariantList vls{QVariant::fromValue(std::forward<Args>(arguments))...};
            success =
                QMetaObject::invokeMethod(object, functionName, Qt::AutoConnection, std::forward<Args>(arguments)...);
        }
        return success;
    }
    std::unordered_map<std::string, std::unique_ptr<QObject>> plugins_;
};
inline AppFramework &appFramework()
{
    return AppFramework::instance();
}
} // namespace AppFrame
