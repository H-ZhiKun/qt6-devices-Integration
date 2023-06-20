#include "AppMetaFlash.h"
#include "AppFramework.h"

bool AppFrame::AppMetaFlash::qmlDominoConnect(const QString &ip, quint16 port)
{
    return appFramework().dominoConnect(ip, port);
}

const QString& AppFrame::AppMetaFlash::getValue1() const
{
    return strValue1;
}
const QString& AppFrame::AppMetaFlash::getValue2() const
{
    return strValue2;
}


void AppFrame::AppMetaFlash::setValue1(QString value)
{
    strValue1 = value;      // 设置属性的新值
    emit valueChanged1(); // 发射信号通知属性已更改
}

void AppFrame::AppMetaFlash::setValue2(QString value)
{
    strValue2 = value;      // 设置属性的新值
    emit valueChanged2(); // 发射信号通知属性已更改
}
