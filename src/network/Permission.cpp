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
    LogWarn("product process: recieve peimisson : {}", result);
    auto iter = mapSignals_.find(result);
    if (iter != mapSignals_.end())
    {
        LogInfo("permission signal: {}", result);
        iter->second();
    }
    std::string code1, code2;
    if (result.length() == 30)
    {
        code1 = result.substr(4, 12);
        code2 = result.substr(16, 12);
    }
    else
    {
        LogError("permission code error.");
    }
    emit codeRight(curNumber, describtion, code1, code2);
}
void Permission::sendQRCode(const uint16_t number, const std::string &code)
{
    curNumber = number;
    std::string cmd = strHead + code + strTail;
    QByteArray byteArray = QByteArray(cmd.c_str(), static_cast<int>(cmd.size()));
    sendData(byteArray);
}
void Permission::pingBehavior()
{
}