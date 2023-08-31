#include "WebsocketClient.h"
#include "Logger.h"

WebsocketClient::WebsocketClient(QObject *parent) : QObject(parent)
{
    client_ = new QWebSocket();
    timerConnect = new QTimer(this);
    timerConnect->setInterval(5000); // 设置重连间隔为5秒
    timerPing = new QTimer(this);
    timerPing->setInterval(5000); // ping间隔为5秒
    connect(timerPing, &QTimer::timeout, this, &WebsocketClient::ping);
    connect(timerConnect, &QTimer::timeout, this, &WebsocketClient::reconnect);
    connect(client_, QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::error), this, &WebsocketClient::onError);
    connect(client_, &QWebSocket::connected, this, &WebsocketClient::onConnected);
    connect(client_, &QWebSocket::disconnected, this, &WebsocketClient::onDisconnected);
    connect(client_, &QWebSocket::textMessageReceived, this, &WebsocketClient::onTextMessageReceived);
}

void WebsocketClient::connectToServer(const std::string &url)
{
    url_ = url;
    client_->open(QString(url_.c_str()));
}

void WebsocketClient::sendData(const std::string &jsonData, const QByteArray &imageBinaryData)
{
    if (!bConnected_)
        return;
    QByteArray combinedData;
    combinedData.append("{-ALGOHead-}");
    combinedData.append(jsonData);
    combinedData.append("{-ALGOBound-}");
    combinedData.append(imageBinaryData.toBase64().toStdString());
    combinedData.append("{-ALGOTail-}");

    qint64 ret = client_->sendTextMessage(combinedData);
    LogInfo("WebsocketClient send data size ={}, ret = {}", combinedData.size(), ret);
}

void WebsocketClient::onConnected()
{
    pingId_ = 0;
    LogInfo("WebsocketClient connected to url:{}", url_);
    bConnected_ = true;
    if (timerConnect)
        timerConnect->stop();
    if (timerPing)
        timerPing->start();
}

void WebsocketClient::onDisconnected()
{
    bConnected_ = false;
    if (timerConnect)
        timerConnect->start();
    if (timerPing)
        timerPing->stop();
}

void WebsocketClient::onTextMessageReceived(const QString &message)
{
    if (message.indexOf("ping") != -1)
        return;
    incompleteData_.append(message);
    LogInfo("onTextMessageReceived {}", message.toStdString());
    while (true)
    {
        int headIndex = incompleteData_.indexOf(strHead_.c_str());
        int tailIndex = incompleteData_.indexOf(strTail_.c_str());

        if (headIndex != -1 && tailIndex != -1)
        {
            if (headIndex < tailIndex)
            {
                QString jsonData = incompleteData_.mid(headIndex + 12, tailIndex - headIndex - 12);
                emit messageReceived(jsonData.toStdString());
                LogInfo("emit messageReceived jsonData={}", jsonData.toStdString());
                incompleteData_ = incompleteData_.mid(tailIndex + 12);
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
    if (timerPing)
        timerPing->stop();
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

void WebsocketClient::ping()
{
    pingId_++;
    QByteArray combinedData;
    std::string pingData = "{\"ping\":" + std::to_string(pingId_) + "}";
    combinedData.append("{-ALGOHead-}");
    combinedData.append(pingData);
    combinedData.append("{-ALGOTail-}");
    client_->sendTextMessage(combinedData);
}
