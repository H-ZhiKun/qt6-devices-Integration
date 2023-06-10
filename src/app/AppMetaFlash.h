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
    public:
        explicit AppMetaFlash(QObject *parent = nullptr) : QObject(parent)
        {
        }
        virtual ~AppMetaFlash() {}
    public slots:
        // qml 调用c++接口区域
        bool qmlDominoConnect(const QString &ip = "", quint16 port = 0);

    private:
        AppMetaFlash(const AppMetaFlash &) = delete;
        AppMetaFlash &operator=(const AppMetaFlash &) = delete;
        AppMetaFlash(AppMetaFlash &&) noexcept(true) = default;
        AppMetaFlash &operator=(AppMetaFlash &&) noexcept(true) = default;
    };
}