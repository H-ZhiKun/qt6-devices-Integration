#ifdef _DEBUG
#include "vld/vld.h"
#endif
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include "string.h"
#include "AppFramework.h"
#include "AppMetaFlash.h"
#include "BaumerManage.h"
#include "Logger.h"
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

    BaumerManage *cameraManage = new BaumerManage();
    cameraManage->updateCameraList();
    // 设置资源路径
    if(cameraManage->cameraList.size() > 0){
        for(int i = 0; i < cameraManage->cameraList.size(); i++){
            cameraManage->cameraList[i]->Init_Parameter();
            cameraManage->cameraList[i]->OpenCamera();

            QString camName = "cameraCtl" + QString::number(i);
            std::string IPName = "QYCamera" + std::to_string(i);
            std::cout << "camName: " << camName.toStdString() << ",  IPName: " << IPName <<std::endl;
            engine.rootContext()->setContextProperty(camName, cameraManage->cameraList[i]);
            engine.addImageProvider(QLatin1String(IPName), cameraManage->cameraList[i]->getImageProvider());
            cameraManage->cameraList[i]->start();
        }        
    }

    engine.loadFromModule("DeviceIntegration", "MainWindow");

    appFramework().run();
    return app.exec();
}
