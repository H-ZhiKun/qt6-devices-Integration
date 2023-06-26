#include "AppFrameworkImpl.h"

#include "AppFramework.h"
#include "AppMetaFlash.h"
#include "DBConnectionPool.h"
#include "Domino.h"
#include "Logger.h"
#include "MysqlConnectionPool.h"
#include "Utils.h"
#include <QBuffer>
#include <QCoreApplication>
#include <QImage>
#include <QObject>
#include <QThread>

using namespace AppFrame;

AppFramework &AppFramework::instance()
{
    return AppFrameworkImpl::instance();
}

AppFrame::AppFrameworkImpl::AppFrameworkImpl()
{
    appMetaFlash_ = new AppMetaFlash;
}

AppFrame::AppFrameworkImpl::~AppFrameworkImpl() noexcept
{
    memoryClean();
}

int AppFrame::AppFrameworkImpl::run()
{
    sWorkPath_ = qApp->applicationDirPath().toStdString();
    Logger::InitLogger(sWorkPath_ + "/logs");
    LOGINFO("AppFrame Run");
    initMysqlTool();
    runDomino();
    runPLC();
    return 0;
}

std::string AppFrame::AppFrameworkImpl::getWorkPath()
{
    return sWorkPath_;
}

bool AppFrame::AppFrameworkImpl::dominoConnect(const QString &ip, quint16 port)
{
    return invokeCpp(domino_, domino_->invokeConnectToServer(), ip, port);
}

AppMetaFlash *AppFrame::AppFrameworkImpl::getAppMetaFlash()
{
    return appMetaFlash_;
}

void AppFrame::AppFrameworkImpl::asyncTask(const std::function<void(void)> &task)
{
    taskPool_->start([task] { task(); });
}

void AppFrame::AppFrameworkImpl::runDomino()
{
    qDebug() << "main thread= " << QThread::currentThreadId();
    domino_ = new Domino();
    QThread *th = new QThread();
    domino_->moveToThread(th);
    //  在子线程中定义槽函数，用于终止对象事件循环
    QObject::connect(qApp, &QCoreApplication::aboutToQuit, th, &QThread::quit);
    QObject::connect(th, &QThread::finished, [this]() { domino_->cleanup(); });

    th->start();
    invokeCpp(domino_, domino_->invokeStartClient(), Q_ARG(QString, "127.0.0.1"), Q_ARG(quint16, 11110));
    lvThread_.push_back(th);

    Json::Value jsVal;
    jsVal["image"] = Utils::imgToBase64("C:/Users/Administrator/Pictures/Camera Roll/123456.jpg");
    jsVal["valve"] = 4;
    jsVal["name"] = "123";
    invokeCpp(appMetaFlash_, appMetaFlash_->invokeSetRealTimeValue(), Q_ARG(QString, appMetaFlash_->strMain),
              Q_ARG(QString, Utils::jsonToString(jsVal).c_str()));
}

void AppFrame::AppFrameworkImpl::runPLC()
{
}

void AppFrame::AppFrameworkImpl::initMysqlTool()
{
    mysqlTool_ = new SqlHelper(new MysqlConnectionPool(20, 5000));
}

void AppFrame::AppFrameworkImpl::timeToClean()
{
    Utils::removeOutdatedFiles(Utils::getCurrentTime(), getWorkPath() + "/logs", -5);
}

void AppFrame::AppFrameworkImpl::memoryClean()
{
    // 退出所有的子线程并回收线程栈资源，堆资源需要后续手动释放
    for (auto ptr : lvThread_)
    {
        if (ptr != nullptr)
        {
            ptr->wait();
            delete ptr;
        }
    }

    if (appMetaFlash_ != nullptr)
    {
        delete appMetaFlash_;
        appMetaFlash_ = nullptr;
    }

    if (mysqlTool_ != nullptr)
    {
        delete mysqlTool_;
        mysqlTool_ = nullptr;
    }
    if (domino_ != nullptr)
    {
        delete domino_;
        domino_ = nullptr;
    }
}
