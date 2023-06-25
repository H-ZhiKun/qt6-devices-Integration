#include "AppMetaFlash.h"
#include "AppFramework.h"

bool AppFrame::AppMetaFlash::qmlDominoConnect(const QString &ip, quint16 port)
{
    return appFramework().dominoConnect(ip, port);
}

void AppFrame::AppMetaFlash::setRealTimeValue(const QString &item, const QString &value)
{
    if (item.isEmpty())
        return;
    std::string key = item.toStdString();
    mapRealTimeData_[key] = std::move(value);
    auto signal = mapSignals_.find(key);
    if (signal != mapSignals_.end())
    {
        signal->second();
    }
}