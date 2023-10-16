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
    size_t commaPos = result.find_first_of(',');
    std::string code, number, code1, code2;
    if (commaPos != std::string::npos)
    {
        number = result.substr(0, commaPos);
        code = result.substr(commaPos + 1); // 添加1来跳过逗号
        if (code.length() == 24)
        {
            LogInfo("product process: recieve peimisson success: code = {}", result);
            code1 = code.substr(0, 12);
            code2 = code.substr(12);
        }
        else
        {
            code1 = result;
            LogInfo("product process: recieve peimisson other data: code = {}", result);
            return;
        }
    }
    else
    {
        // 处理没有逗号的情况
        code1 = result;
        LogError("product process: recieve peimisson error: code = {}", result);
        return;
    }
    emit codeRight(number, code1, code2);
}
void Permission::sendQRCode(const uint32_t num, const std::string &code)
{
    std::string cmd = strHead + std::to_string(num) + "," + code + strTail;
    QByteArray byteArray = QByteArray(cmd.c_str(), static_cast<int>(cmd.size()));
    qDebug() << "send data: " << byteArray;
    sendData(byteArray);
}
void Permission::pingBehavior()
{
    // static int num = 1;
    // std::string code = "HTTPS://N.PLSBD.COM/W/?W=W1DF3BF7C1547603";
    // sendQRCode(num, code);
    // num++;
}