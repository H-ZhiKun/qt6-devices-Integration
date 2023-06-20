#ifdef _DEBUG
#include "vld/vld.h"
#endif
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "AppFramework.h"
#include "AppMetaFlash.h"
using namespace AppFrame;

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    
    QQmlApplicationEngine engine;
    // 注册MyObject类
    qmlRegisterType<AppMetaFlash>("AppFrame", 1, 0, "AppMetaFlash");
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []()
        { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.rootContext()->setContextProperty("appMetaFlash", appFramework().getAppMetaFlash());

    engine.loadFromModule("DeviceIntegration", "Main");
    appFramework().run();
    return app.exec();
}
