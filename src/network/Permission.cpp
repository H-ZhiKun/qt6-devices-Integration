#include "Permission.h"
#include "Logger.h"
#include "QString"
#include "Utils.h"

Permission::Permission(QObject *parent) : TCPClient(parent)
{
}

Permission::~Permission()
{
}

void Permission::dealing(std::vector<unsigned char> buffer)
{
    std::string result(buffer.begin(), buffer.end());
    LogInfo("Permission::dealing : {}", result);
    auto vRecvData = Utils::splitString(result, "\r\n");
    for (const auto &info : vRecvData)
    {
        auto vResult = Utils::splitString(info, ",");
        if (vResult.size() == 5)
        {
            return;
        }
        size_t commaPos = info.find_first_of(',');
        std::string code;
        uint32_t number = 0;
        if (commaPos != std::string::npos)
        {
            QString strNumber = QString().fromStdString(info.substr(0, commaPos));
            number = strNumber.replace("FFFF", "").toInt();
            code = info.substr(commaPos + 1); // 添加1来跳过逗号
            emit codeRight(number, code);
            LogInfo("Permission::add count : {}: {}, info: {}, result: {}", number, code, info, result);
        }
    }
}

void Permission::sendQRCode(const uint32_t num, const std::string &code)
{
    std::string cmd = strHead + std::to_string(num) + "," + code + strTail;
    QByteArray byteArray = QByteArray(cmd.c_str(), static_cast<int>(cmd.size()));
    LogInfo("sendQRCode: {}", byteArray.toStdString());
    sendData(byteArray);
}
void Permission::pingBehavior()
{
    // static int num = 1;
    // std::string code = "HTTPS://N.PLSBD.COM/W/?W=W1DF3BF7C1547603";
    // sendQRCode(num, code);
    // num++;
}