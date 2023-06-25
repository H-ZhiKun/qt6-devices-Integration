#pragma once
#include "AppFramework.h"
#include "AppMetaFlash.h"
#include "Domino.h"
#include "Logger.h"
#include "ModbusClient.h"
#include "SqlHelper.h"
#include <QThreadPool>
#include <memory>
#include <mutex>
#include <thread>
namespace AppFrame
{
class AppFrameworkImpl final : public AppFramework
{
  public:
    AppFrameworkImpl();
    ~AppFrameworkImpl() noexcept override;
    inline static AppFrameworkImpl &instance()
    {
        static AppFrameworkImpl instance;
        return instance;
    }
    int run() override;
    std::string getWorkPath() override;
    bool dominoConnect(const QString &ip = "", quint16 port = 0) override;
    AppMetaFlash *getAppMetaFlash() override;
    void asyncTask(const std::function<void(void)> &task) override;

  private:
    // 私有接口区域
    void runDomino();
    void runPLC();
    void initMysqlTool();
    void timeToClean();
    void memoryClean();
    // 私有变量区域
    std::string sWorkPath_{};
    QThreadPool *taskPool_ = QThreadPool().globalInstance();

    std::list<QThread *> lvThread_;
    // Module 组装区域
    AppMetaFlash *appMetaFlash_ = nullptr;
    Domino *domino_ = nullptr;
    SqlHelper *mysqlTool_ = nullptr;
    ModbusClient *mbsPLC_ = nullptr;

  public:
    // 调用qml 对象函数工具

    // 调用C++ 对象函数工具
    bool invokeCpp(QObject *object, const QString &functionName, const QVariantList &args)
    {
        bool success = false;
        if (object)
        {
            success = QMetaObject::invokeMethod(object, functionName.toUtf8().constData(), Qt::QueuedConnection, args);
            if (!success)
                LOGERROR("method={} is called failed", functionName.toStdString());
        }
        else
        {
            LOGERROR("object is nullptr");
        }
        return success;
    }
    template <typename... Args> bool invokeCpp(QObject *object, const QString &functionName, Args &&...arguments)
    {
        bool success = false;
        if (object)
        {
            success = QMetaObject::invokeMethod(object, functionName.toUtf8().constData(), Qt::QueuedConnection,
                                                std::forward<Args>(arguments)...);
            if (success == false)
                LOGERROR("method={} is called failed", functionName.toStdString());
        }
        else
        {
            LOGERROR("object is nullptr");
        }
        return success;
    }
};
} // namespace AppFrame
