#include "Domino.h"
#include "Logger.h"
Domino::Domino(QObject *parent) : TCPClient(parent)
{
}

Domino::~Domino()
{
}

void Domino::dealing(std::vector<unsigned char> buffer)
{
    std::string str(buffer.begin(), buffer.end());
    LogInfo("Receive painter info: {}", str);
    // if (buffer.empty())
    // {
    //     LogInfo("Receive painter info is empty");
    //     return;
    // }
    // if (buffer.size() != 5)
    // {
    //     std::string str(byteVector.begin(), byteVector.end());
    //     LogInfo("Receive painter info faile: {}", str);
    //     return;
    // }
    // if (buffer[0] != 0x02 || buffer[1] != 0x05 || buffer[4] != 0x03)
    // {
    //     std::string str(byteVector.begin(), byteVector.end());
    //     LogInfo("Receive painter info faile: {}", str);
    //     return;
    // }
    // if (buffer[2] == 0x02)
    //     LogError("Receive painter info faile: CodeError");
    // if (buffer[2] == 0x01)
    // {
    //     ResponseCode code = static_cast<ResponseCode>(buffer[3]);
    //     switch (code)
    //     {
    //     case ResponseCode::CodeNomal: {
    //         LogInfo("CodeNomal");
    //         break;
    //     }
    //     case ResponseCode::CodeDisconnect: {
    //         LogInfo("CodeDisconnect");
    //         break;
    //     }
    //     case ResponseCode::CodePrinting: {
    //         LogInfo("CodePrinting");
    //         break;
    //     }
    //     default:
    //         break;
    //     }
    // }
}

void Domino::pingBehavior()
{
    // dominoCheck();
}

void Domino::dominoCheck()
{
    sendData(QByteArray(reinterpret_cast<const char *>(cmdCheck.data()), static_cast<int>(cmdCheck.size())));
}

void Domino::dominoPrint(std::string code1, std::string code2)
{
    LogInfo("product process: send logistic: code1-{}, code2-{}", code1, code2);
    if (code1.empty() || code2.empty())
        return;
    int index = 3;
    for (unsigned char val : code1)
    {
        cmdPrint[index] = val;
        index++;
    }
    index = 16;
    for (unsigned char val : code2)
    {
        cmdPrint[index] = val;
        index++;
    }
    sendData(QByteArray(reinterpret_cast<const char *>(cmdPrint.data()), static_cast<int>(cmdPrint.size())));
}