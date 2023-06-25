#include "TCPClient.h"
#include "Logger.h"
#include <QCoreApplication>
#include <QMetaEnum>
#include <QThread>

TCPClient::TCPClient(QObject *parent) : QObject(parent)
{
}

TCPClient::~TCPClient()
{
}

void TCPClient::cleanup()
{
    if (pingHolder != nullptr)
    {
        pingHolder->stop(); // 使用 deleteLater() 来安全删除对象
        pingHolder->deleteLater();
    }
    if (reconnectTimer != nullptr)
    {
        reconnectTimer->stop();
        reconnectTimer->deleteLater();
    }
    if (tcpSocket != nullptr)
    {
        tcpSocket->close();
        tcpSocket->deleteLater();
    }
}

void TCPClient::startClient(const QString &host, quint16 port)
{
    qDebug() << "this thread= " << QThread::currentThreadId();
    serverHost = host;
    serverPort = port;
    tcpSocket = new QTcpSocket(this);
    pingHolder = new QTimer(this);
    reconnectTimer = new QTimer(this);
    pingHolder->setInterval(5000);     // ping设置重连间隔为5秒
    reconnectTimer->setInterval(5000); // 设置重连间隔为5秒
    connect(reconnectTimer, &QTimer::timeout, this, &TCPClient::reconnect);
    connect(pingHolder, &QTimer::timeout, this, &TCPClient::ping);
    connect(tcpSocket, &QTcpSocket::connected, this, &TCPClient::onConnected);
    connect(tcpSocket, &QTcpSocket::disconnected, this, &TCPClient::onDisconnected);
    connect(tcpSocket, &QAbstractSocket::errorOccurred, this, &TCPClient::onError);
    connect(tcpSocket, &QTcpSocket::readyRead, this, &TCPClient::onReadyRead);
    connectToServer();
}

void TCPClient::connectToServer(const QString &host, quint16 port)
{
    if (!host.isEmpty())
        serverHost = host;
    if (port != 0)
        serverPort = port;
    if (tcpSocket != nullptr)
    {
        tcpSocket->connectToHost(serverHost, serverPort);
    }
}

void TCPClient::sendData(const QByteArray &data)
{
    if (tcpSocket && tcpSocket->state() == QAbstractSocket::ConnectedState)
    {
        tcpSocket->write(data);
        tcpSocket->flush();
    }
}

void TCPClient::pingEnable(bool bEnable)
{
    if (pingHolder == nullptr)
        return;
    if (bEnable)
        pingHolder->start();
    else
        pingHolder->stop();
}

QString TCPClient::socketErrorToString(QAbstractSocket::SocketError socketError)
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

void TCPClient::reconnect()
{
    if (tcpSocket && tcpSocket->state() == QAbstractSocket::UnconnectedState)
    {
        // 断开连接后重新连接到服务器
        LOGINFO("reconnect to {}", serverHost.toStdString());
        connectToServer();
    }
}

void TCPClient::onConnected()
{
    LOGINFO("Connected to {}", serverHost.toStdString());
    // 连接成功后停止重连计时器
    if (reconnectTimer)
        reconnectTimer->stop();
    pingEnable(true);
}

void TCPClient::onDisconnected()
{
    LOGERROR("Disconnected from {}", serverHost.toStdString());
    // 断开连接后启动重连计时器
    if (reconnectTimer)
        reconnectTimer->start();
    pingEnable(false);
}

void TCPClient::onError(QAbstractSocket::SocketError errCode)
{
    pingEnable(false);
    LOGERROR("{} Socket error: {}", serverHost.toStdString(), socketErrorToString(errCode).toStdString());
    // 发生错误后启动重连计时器
    if (reconnectTimer)
        reconnectTimer->start();
}

void TCPClient::onReadyRead()
{
    if (tcpSocket == nullptr)
        return;
    QByteArray recvData = tcpSocket->readAll();
    // 子类处理抽象接口
    dealing(std::vector<unsigned char>(recvData.begin(), recvData.end()));
}

void TCPClient::ping()
{
    pingBehavior();
}
