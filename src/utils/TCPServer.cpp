#include "TCPServer.h"
#include "Logger.h"
#include <QMetaEnum>

TCPServer::TCPServer(QObject *parent) : QObject(parent)
{
}

TCPServer::~TCPServer() noexcept
{
    stopServer();
}

void TCPServer::startServer(quint16 port)
{
    serverPort = port;
    tcpServer = new QTcpServer(this);
    connect(tcpServer, &QTcpServer::newConnection, this, &TCPServer::onNewConnection);

    if (!tcpServer->listen(QHostAddress::Any, serverPort))
    {
        LogError("Server failed to start: {}", tcpServer->errorString().toStdString());
        return;
    }

    LogInfo("Server started on port {}", serverPort);
}

void TCPServer::stopServer()
{
    if (tcpServer)
    {
        foreach (QTcpSocket *client, clients)
        {
            client->disconnectFromHost();
            client->close();
        }
        tcpServer->close();
        tcpServer->deleteLater();
    }
}

bool TCPServer::sendData(QTcpSocket *client, const QByteArray &data)
{
    bool ret = false;
    if (client && client->state() == QAbstractSocket::ConnectedState)
    {
        client->write(data);
        client->flush();
        ret = true;
    }
    return ret;
}

void TCPServer::onNewConnection()
{
    while (tcpServer->hasPendingConnections())
    {
        QTcpSocket *client = tcpServer->nextPendingConnection();
        clients.push_back(client);
        connect(client, &QTcpSocket::disconnected, this, &TCPServer::onClientDisconnected);
        connect(client, &QAbstractSocket::errorOccurred, this, [=, this](QAbstractSocket::SocketError socketError) {
            LogError("Client {} error: {}", client->peerAddress().toString().toStdString(),
                     socketErrorToString(socketError).toStdString());
        });
        connect(client, &QTcpSocket::readyRead, this, &TCPServer::onReadyRead);
        LogInfo("New client connected: {}", client->peerAddress().toString().toStdString());
        clientConnected(client);
    }
}

void TCPServer::onClientDisconnected()
{
    QTcpSocket *client = qobject_cast<QTcpSocket *>(sender());
    if (client)
    {
        clients.erase(std::remove(clients.begin(), clients.end(), client), clients.end());
        LogInfo("Client disconnected: {}", client->peerAddress().toString().toStdString());
        clientDisconnected(client);
        client->deleteLater();
    }
}

void TCPServer::onReadyRead()
{
    QTcpSocket *client = qobject_cast<QTcpSocket *>(sender());
    if (client)
    {
        QByteArray recvData = client->readAll();
        dealing(client, std::vector<unsigned char>(recvData.begin(), recvData.end()));
    }
}

QString TCPServer::socketErrorToString(QAbstractSocket::SocketError socketError)
{
    const QMetaObject &metaObject = QAbstractSocket::staticMetaObject;
    int index = metaObject.indexOfEnumerator("SocketError");
    QMetaEnum metaEnum = metaObject.enumerator(index);
    const char *errorString = metaEnum.valueToKey(socketError);
    if (errorString)
    {
        return QString::fromLatin1(errorString);
    }
    else
    {
        return QString();
    }
}
