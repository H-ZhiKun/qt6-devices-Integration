/*
 * @Author: A炼 834022125@qq.com
 * @Date: 2023-06-10 14:28:59
 * @LastEditors: A炼 834022125@qq.com
 * @LastEditTime: 2023-06-10 16:12:33
 * @FilePath: \DeviceIntegration\src\main.cpp
 * @Description: 
 * 
 * Copyright (c) 2023 by ${git_name_email}, All Rights Reserved. 
 */
#ifdef _DEBUG
#include "vld/vld.h"
#endif
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "AppFramework.h"
#include "AppMetaFlash.h"
#include <QQuickStyle>
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
    // 设置资源路径

    engine.loadFromModule("DeviceIntegration", "App");
    appFramework().run(&engine);
    return app.exec();
}
