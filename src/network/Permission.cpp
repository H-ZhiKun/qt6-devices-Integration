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
    std::string result(buffer.begin() + 4, buffer.end() - 2);
    std::string number = result.substr(0, result.find_first_of(','));
    std::string code = result.substr(result.find_first_of(','));
    std::string code1, code2;
    if (code.length() == 24)
    {
        LogInfo("product process: recieve peimisson success: code = {}", result);
        code1 = result.substr(0, 12);
        code2 = result.substr(12);
    }
    else
    {
        code1 = code;
        LogError("product process: recieve peimisson error: code = {}", result);
    }
    emit codeRight(number, code1, code2);
}
void Permission::sendQRCode(const std::string &code, const std::string &num)
{
    std::string cmd = strHead + num + "," + code + strTail;
    QByteArray byteArray = QByteArray(cmd.c_str(), static_cast<int>(cmd.size()));
    sendData(byteArray);
}
void Permission::pingBehavior()
{
}