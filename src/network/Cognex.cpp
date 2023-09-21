#include "Cognex.h"

Cognex::Cognex(QObject *parent)
{
}

void Cognex::dealing(std::vector<unsigned char> buffer)
{
    std::string str(buffer.begin(), buffer.end());
    emit ReadQRCode(str);
}

// void Cognex::pingBehavior()
// {
//     scanCode();
// }

void Cognex::scanCode()
{
    sendData("+");
}

void Cognex::scanStop()
{
    sendData("-");
}

// bool Cognex::connectToServer(const QString &serverAddress, quint16 port)
// {
//     socket->connectToHost(serverAddress, port);
//     return socket->waitForConnected();
// }

Cognex::~Cognex() noexcept
{
    sendData("-");
}