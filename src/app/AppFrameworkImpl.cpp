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

	asyncTask([this]
			  {
							int i = 0;
							while(1)
							{
			invokeCpp(appMetaFlash_, appMetaFlash_->invokeSetValue1(), QString::number(i)); 
			if(i > 10000) i = 0;
			i++;
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
							} });
	asyncTask([this]
			  {
							int i = 10000;
							while(1)
							{
			invokeCpp(appMetaFlash_, appMetaFlash_->invokeSetValue2(), QString::number(i)); 
			if(i < 0) i = 10000;
			i--;
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
							} });
	return invokeCpp(domino_, domino_->invokeConnectToServer(), ip, port);
}

AppMetaFlash *AppFrame::AppFrameworkImpl::getAppMetaFlash()
{
	return appMetaFlash_;
}

void AppFrame::AppFrameworkImpl::asyncTask(const std::function<void(void)> &task)
{
	taskPool_->start([task]
					 { task(); });
}

void AppFrame::AppFrameworkImpl::runDomino()
{
	qDebug() << "main thread= " << QThread::currentThreadId();
	domino_ = new Domino();
	QThread *th = new QThread();
	domino_->moveToThread(th);
	//  在子线程中定义槽函数，用于终止对象事件循环
	QObject::connect(qApp, &QCoreApplication::aboutToQuit, th, &QThread::quit);
	QObject::connect(th, &QThread::finished, [this]()
					 { domino_->cleanup(); });
	th->start();
	invokeCpp(domino_, domino_->invokeStartClient(), Q_ARG(QString, "127.0.0.1"), Q_ARG(quint16, 11110));
	lvThread_.push_back(th);
}

void AppFrame::AppFrameworkImpl::runPLC()
{
	mbsPLC_ = new ModbusClient("127.0.0.1", 502);
	std::vector<ModbusReadArgument> args;
	for (int i = 1; i < 6; i++)
	{
		ModbusReadArgument arg;
		arg.addr = 32;
		arg.offset = 1;
		arg.clock = 500;
		args.emplace_back(arg);
	}
	mbsPLC_->work(args);
}

void AppFrame::AppFrameworkImpl::initMysqlTool()
{
	mysqlTool_ = new SqlHelper(new MysqlConnectionPool(20, 5000));
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
	if (mbsPLC_ != nullptr)
	{
		delete mbsPLC_;
		mbsPLC_ = nullptr;
	}
}
