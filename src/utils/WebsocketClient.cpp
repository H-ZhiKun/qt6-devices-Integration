#include "WebsocketClient.h"
#include "Logger.h"

WebsocketClient::WebsocketClient(QObject *parent) : QObject(parent)
{
    client_ = new QWebSocket();
    timerConnect = new QTimer(this);
    timerConnect->setInterval(5000); // ping设置重连间隔为5秒
    connect(timerConnect, &QTimer::timeout, this, &WebsocketClient::reconnect);
    connect(client_, QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::error), this, &WebsocketClient::onError);
    connect(client_, &QWebSocket::connected, this, &WebsocketClient::onConnected);
    connect(client_, &QWebSocket::disconnected, this, &WebsocketClient::onDisconnected);
    connect(client_, &QWebSocket::binaryMessageReceived, this, &WebsocketClient::onBinaryMessageReceived);
}

void WebsocketClient::connectToServer(const std::string &url)
{
    url_ = url;
    client_->open(QString(url_.c_str()));
}

void WebsocketClient::sendData(const std::string &jsonData, const QByteArray &imageBinaryData)
{
    QByteArray combinedData;
    combinedData.append("{-ALGOHead-}");
    combinedData.append(jsonData);
    combinedData.append("{-ALGOBound-}");
    combinedData.append(imageBinaryData);
    combinedData.append("{-ALGOTail-}");

    client_->sendBinaryMessage(combinedData);
}

void WebsocketClient::onConnected()
{
    LogInfo("WebsocketClient connected to url:{}", url_);
    bConnected_ = true;
    if (timerConnect)
        timerConnect->stop();
}

void WebsocketClient::onDisconnected()
{
    bConnected_ = false;
    if (timerConnect)
        timerConnect->start();
}

void WebsocketClient::onBinaryMessageReceived(const QByteArray &message)
{
    incompleteData_.append(message);

    while (true)
    {
        int headIndex = incompleteData_.indexOf(strHead_);
        int tailIndex = incompleteData_.indexOf(strTail_);

        if (headIndex != -1 && tailIndex != -1)
        {
            if (headIndex < tailIndex)
            {
                QByteArray jsonData = incompleteData_.mid(headIndex + 11, tailIndex - headIndex - 11);
                emit messageReceived(jsonData.toStdString());

                incompleteData_ = incompleteData_.mid(tailIndex + 11);
            }
            else
            {
                incompleteData_ = incompleteData_.mid(headIndex);
            }
        }
        else
        {
            break;
        }
    }
}

void WebsocketClient::onError(QAbstractSocket::SocketError errCode)
{
    if (timerConnect)
        timerConnect->start();
    switch (errCode)
    {
    case QAbstractSocket::ConnectionRefusedError:
        LogError("WebsocketClient {} Connection refused", url_);
        // Handle connection refused error
        break;

    case QAbstractSocket::RemoteHostClosedError:
        LogError("WebsocketClient {} Remote host closed the connection", url_);
        // Handle remote host closed connection error
        break;

        // Handle other error cases as needed...

    default:
        LogError("WebsocketClient {} Socket error: {}", url_, client_->errorString().toStdString());
        break;
    }
}

void WebsocketClient::reconnect()
{
    client_->open(QString(url_.c_str()));
    LogInfo("WebsocketClient reconnect to url:{}", url_);
}
