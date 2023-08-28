#ifdef _DEBUG
#include "vld/vld.h"
#endif
#include "AppFramework.h"
#include "AppMetaFlash.h"
#include "ImagePainter.h"
#include "Logger.h"
#include "string.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include <QSharedMemory>
#include <iostream>
using namespace AppFrame;

int main(int argc, char *argv[])
{
    cv::Mat image(400, 400, CV_8UC3, cv::Scalar(255, 255, 255));
    cv::putText(image, "xiaoweiguo", cv::Point(image.rows / 2, image.cols / 2), cv::FONT_HERSHEY_SIMPLEX, 2,
                cv::Scalar(0, 0, 255), 1, 8, false);

    // 显示图像
    cv::imshow("Image with Text", image);
    cv::waitKey(0);
    return 0;
    // QGuiApplication app(argc, argv);
    // // 设置一个唯一的键名，用于标识共享内存区域
    // QSharedMemory sharedMemory("AppFrame-device-integration");

    // // 检查是否已经有一个实例正在运行
    // if (sharedMemory.attach())
    // {
    //     // 如果已经有一个实例正在运行，则不启动新的实例，直接退出
    //     return 0;
    // }

    // // 创建共享内存区域，并将其关联到当前应用程序的进程空间
    // if (!sharedMemory.create(1))
    // {
    //     // 创建共享内存失败，可能是由于已经有一个实例在运行
    //     // 在这里处理错误情况
    //     return 1;
    // }

    // QQmlApplicationEngine engine;
    // // 注册MyObject类
    // qmlRegisterType<AppMetaFlash>("AppFrame", 1, 0, "AppMetaFlash");
    // qmlRegisterType<ImagePainter>("AppFrame", 1, 0, "ImagePainter");
    // QObject::connect(
    //     &engine, &QQmlApplicationEngine::objectCreationFailed, &app, []() { QCoreApplication::exit(-1); },
    //     Qt::AutoConnection);

    // engine.rootContext()->setContextProperty("appMetaFlash", &AppMetaFlash::instance());
    // engine.rootContext()->setContextProperty("appdir", qApp->applicationDirPath()); // 获取当前路径
    // engine.loadFromModule("DeviceIntegration", "MainWindow");
    // app.setWindowIcon(QIcon("file:///" + qApp->applicationDirPath() + "/ico/logo.ico")); // 添加图标
    // // 相机序号是从0开始的
    // appFramework().storeImagePainter(DisplayWindows::LocationCamera,
    //                                  engine.rootObjects().first()->findChild<QObject *>("locationCamera"));
    // appFramework().storeImagePainter(DisplayWindows::CodeCheckCamera,
    //                                  engine.rootObjects().first()->findChild<QObject *>("codeCheckCamera"));
    // appFramework().storeImagePainter(DisplayWindows::LocateCheckCamera,
    //                                  engine.rootObjects().first()->findChild<QObject *>("locateCheckCamera"));
    // appFramework().run();
    // return app.exec();
}
