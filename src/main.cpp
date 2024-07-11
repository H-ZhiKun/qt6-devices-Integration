#ifdef _DEBUG
#include <vld/vld.h>
#endif
#include "AppFramework.h"
#include <QGuiApplication>
#include <QIcon>
#include <QQuickStyle>
#include <QSharedMemory>
using namespace AppFrame;

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    // 设置一个唯一的键名，用于标识共享内存区域
    QSharedMemory sharedMemory("AppFrame-device-integration");

    // 检查是否已经有一个实例正在运行
    if (sharedMemory.attach())
    {
        // 如果已经有一个实例正在运行，则不启动新的实例，直接退出
        return 0;
    }

    // 创建共享内存区域，并将其关联到当前应用程序的进程空间
    if (!sharedMemory.create(1))
    {
        // 创建共享内存失败，可能是由于已经有一个实例在运行
        // 在这里处理错误情况
        return 1;
    }

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreationFailed, &app, []() { QCoreApplication::exit(-1); },
        Qt::AutoConnection);
    app.setWindowIcon(QIcon("file:///" + qApp->applicationDirPath() + "/ico/logo4.ico")); // 添加图标
    appFramework().run(&engine);
    int retCode = app.exec();
    appFramework().stop();
    return retCode;
}
