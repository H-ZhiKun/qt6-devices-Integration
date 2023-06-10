#include "AppMetaFlash.h"
#include "AppFramework.h"

bool AppFrame::AppMetaFlash::qmlDominoConnect(const QString &ip, quint16 port)
{
    return appFramework().dominoConnect(ip, port);
}