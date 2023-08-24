#include "Permission.h"
#include "Logger.h"
#include <QDebug>
Permission::Permission(QObject *parent) : TCPClient(parent)
{
    mapSignals_[strCodeError] = std::bind([this] { emit codeError(); });
    mapSignals_[strCodeNull] = std::bind([this] { emit codeNull(); });
    mapSignals_[strSysEor1] = std::bind([this] { emit sysError(); });
    mapSignals_[strSystemOk] = std::bind([this] { emit systemOk(); });
}

Permission::~Permission()
{
}

void Permission::dealing(std::vector<unsigned char> buffer)
{
    std::string result(buffer.begin(), buffer.end());
    qDebug() << result.c_str();
    auto iter = mapSignals_.find(result);
    if (iter != mapSignals_.end())
    {
        LogInfo("permission signal: {}", result);
        iter->second();
    }
}
void Permission::sendQRCode(const std::string &code)
{
    std::string cmd = strHead + code + strTail;
    sendData(QByteArray(cmd.c_str(), static_cast<int>(cmd.size())));
}
void Permission::pingBehavior()
{
}