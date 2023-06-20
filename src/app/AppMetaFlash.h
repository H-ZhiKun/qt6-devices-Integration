#pragma once
#include <QObject>
#include <QCoreApplication>
#include <QString>
#include <QVariant>
#include "Logger.h"

namespace AppFrame
{
    class AppMetaFlash : public QObject
    {
        Q_OBJECT
        // c++ 调用 qml 需要在本类中定义三个位置的方法
        // qml (1) 声明  Q_PROPERTY（属性类型 属性名称 READ get方法 NOTIFY 信号函数名称）
        // 注意：属性类型在qml中可以直接使用的，属性名称是痛过qml中可以直接访问的。
        // READ 方法是必须要声明且实现的，qml中调用需要
        // NOTIFY 只需要声明一个信号函数即可。
        Q_PROPERTY(QString strValue1 READ getValue1 NOTIFY valueChanged1) 
        Q_PROPERTY(QString strValue2 READ getValue2 NOTIFY valueChanged2)
    public:
        // qml (2) 声明  Q_PROPERTY中 READ 方法
        const QString& getValue1() const;
        const QString& getValue2() const;
    signals:
        // qml (3) 声明  Q_PROPERTY中的信号函数
        void valueChanged1();
        void valueChanged2();
    public:
        explicit AppMetaFlash(QObject *parent = nullptr) : QObject(parent)
        {
        }
        virtual ~AppMetaFlash() {}
    public slots:
        // qml 调用c++接口区域
        bool qmlDominoConnect(const QString &ip = "", quint16 port = 0);
        // cpp 调用qml接口区域
        // qml (4) 声明 并实现 槽函数 在cpp中进行调用
        void setValue1(QString value);
        void setValue2(QString value);

        QString invokeSetValue1(){return "setValue1";}
        QString invokeSetValue2(){return "setValue2";}
    private:
        // qml (5) Q_PROPERTY 中 属性类型 属性名称 必须一致
        QString strValue1 = "1";
        QString strValue2 = "1";
        AppMetaFlash(const AppMetaFlash &) = delete;
        AppMetaFlash &operator=(const AppMetaFlash &) = delete;
        AppMetaFlash(AppMetaFlash &&) noexcept(true) = default;
        AppMetaFlash &operator=(AppMetaFlash &&) noexcept(true) = default;
    };
}