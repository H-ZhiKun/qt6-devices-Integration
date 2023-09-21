#include "Permission.h"
#include "Logger.h"
#include <QDebug>
Permission::Permission(QObject *parent) : TCPClient(parent)
{
}

Permission::~Permission()
{
}

void Permission::dealing(std::vector<unsigned char> buffer)
{
    std::string result(buffer.begin(), buffer.end());
    std::string code1, code2;
    if (result.length() == 30)
    {
        LogInfo("product process: recieve peimisson success: code = {}", result);
        code1 = result.substr(4, 12);
        code2 = result.substr(16, 12);
    }
    else
    {
        code1 = result;
        LogError("product process: recieve peimisson error: code = {}", result);
    }
    emit codeRight(code1, code2);
}
void Permission::sendQRCode(const std::string &code)
{
    std::string cmd = strHead + code + strTail;
    QByteArray byteArray = QByteArray(cmd.c_str(), static_cast<int>(cmd.size()));
    sendData(byteArray);
}
void Permission::pingBehavior()
{
}