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
    auto iter = mapSignals_.find(result);
    if (iter != mapSignals_.end())
    {
        LogInfo("permission signal: {}", result);
        iter->second();
    }
    else if (result.length() == 30)
    {
        std::string code1 = result.substr(4, 12);
        std::string code2 = result.substr(16, 12);
        LogInfo("code1 = ", code1);
        LogInfo("code2 = ", code2);
        emit codeRight(code1, code2);
    }
    else
    {
        LogError("permission code error.");
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