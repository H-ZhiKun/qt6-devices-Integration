#include "AppFrameworkImpl.h"
#include "AppMetaFlash.h"
#include "Logger.h"
#include "Domino.h"
#include <QObject>
#include <QThread>
#include "Utils.h"
#include "DBConnectionPool.h"
#include "MysqlConnectionPool.h"
#include <QCoreApplication>
#include "AppFramework.h"

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

int AppFrame::AppFrameworkImpl::run(QQmlApplicationEngine *engine)
{
	engine_ = engine;
	sWorkPath_ = qApp->applicationDirPath().toStdString();
	Logger::InitLogger(sWorkPath_ + "/logs");
	LOGINFO("AppFrame Run");
	initMysqlTool();
	runDomino();
	return 0;
}

std::string AppFrame::AppFrameworkImpl::getWorkPath()
{
	return sWorkPath_;
}

bool AppFrame::AppFrameworkImpl::dominoConnect(const QString &ip, quint16 port)
{
	QObject *button = engine_->rootObjects().first()->findChild<QObject *>("button1");
	if (button)
	{
		QVariantList myList;
		myList.append(3);


		myList.append("zk");

		
		myList.append(false);
		button->setProperty("myProperty", myList);
	}
	return invokeCpp(domino_, domino_->getNameConnectToServer(), ip, port);
}

AppMetaFlash *AppFrame::AppFrameworkImpl::getAppMetaFlash()
{
	return appMetaFlash_;
}

void AppFrame::AppFrameworkImpl::runDomino()
{
	qDebug() << "main thread= " << QThread::currentThreadId();
	domino_ = new Domino();
	QThread *th = new QThread();
	domino_->moveToThread(th);
	//  在子线程中定义槽函数，用于终止对象事件循环
	QObject::connect(qApp, &QCoreApplication::aboutToQuit, th, &QThread::quit);
	QObject::connect(th, &QThread::finished, [this](){ domino_->cleanup(); });
	th->start();
	invokeCpp(domino_, domino_->getNameStartClient(), Q_ARG(QString, "127.0.0.1"), Q_ARG(quint16, 11110));
	lvThread_.push_back(th);
}

void AppFrame::AppFrameworkImpl::initMysqlTool()
{
	mysqlTool_ = new SqlHelper(new MysqlConnectionPool(20, 5000));
	taskPool_->start([this]
					 {
	QStringList fields;
	fields << "id INT PRIMARY KEY AUTO_INCREMENT"
		   << "name VARCHAR(100) NOT NULL UNIQUE"
		   << "age INT NOT NULL";
	mysqlTool_->createTable("test", fields); });
}

void AppFrame::AppFrameworkImpl::timeToClean()
{
	Utils::removeOutdatedFiles(
		Utils::getCurrentTime(),
		getWorkPath() + "/logs",
		-5);
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
