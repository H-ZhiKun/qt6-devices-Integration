#pragma once
#include "AppFramework.h"
#include <QThreadPool>
#include "AppMetaFlash.h"
#include "Logger.h"
#include <memory>
#include "Domino.h"
#include "SqlHelper.h"
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
        int run(QQmlApplicationEngine *engine) override;
        std::string getWorkPath() override;
        bool dominoConnect(const QString &ip = "", quint16 port = 0) override;
        AppMetaFlash *getAppMetaFlash() override;

    private:
        // 私有接口区域
        void runDomino();
        void initMysqlTool();
        void timeToClean();
        void memoryClean();
        // 私有变量区域
        QQmlApplicationEngine *engine_ = nullptr;
        std::string sWorkPath_{};
        QThreadPool *taskPool_ = QThreadPool().globalInstance();

        std::list<QThread *> lvThread_;
        // Module 组装区域
        AppMetaFlash *appMetaFlash_ = nullptr;
        Domino *domino_ = nullptr;
        SqlHelper *mysqlTool_ = nullptr;

    protected:
        // 调用qml 对象函数工具

        // 调用C++ 对象函数工具
        bool invokeCpp(QObject *object, const QString &functionName, const QVariantList &args)
        {
            bool success = false;
            if (object)
            {
                success = QMetaObject::invokeMethod(object, functionName.toUtf8().constData(),
                                                    Qt::QueuedConnection, args);
                if (!success)
                    LOGERROR("method={} is called failed", functionName.toStdString());
            }
            else
            {
                LOGERROR("object is nullptr");
            }
            return success;
        }
        template <typename... Args>
        bool
        invokeCpp(QObject *object, const QString &functionName, Args &&...arguments)
        {
            bool success = false;
            if (object)
            {
                success = QMetaObject::invokeMethod(object, functionName.toUtf8().constData(),
                                                    Qt::QueuedConnection, std::forward<Args>(arguments)...);
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
}
