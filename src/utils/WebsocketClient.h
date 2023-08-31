#ifndef WEBSOCKETCLIENT_H
#define WEBSOCKETCLIENT_H

#include <QObject>
#include <QTimer>
#include <QUrl>
#include <QWebSocket>
#include <string>

class WebsocketClient : public QObject
{
    Q_OBJECT

  public:
    explicit WebsocketClient(QObject *parent = nullptr);

    void connectToServer(const std::string &url);
    void sendData(const std::string &jsonData, const QByteArray &imageBinaryData);

  signals:
    void messageReceived(const std::string &jsonObject);

  private slots:
    void onConnected();
    void onDisconnected();
    void onTextMessageReceived(const QString &message);
    void onError(QAbstractSocket::SocketError errCode);
    void reconnect();
    void ping();

  private:
    std::string url_;
    QWebSocket *client_;
    QString incompleteData_;
    std::string strHead_{"{-ALGOHead-}"};
    std::string strBound_{"{-ALGOBound-}"};
    std::string strTail_{"{-ALGOTail-}"};
    QTimer *timerConnect = nullptr;
    QTimer *timerPing = nullptr;
    bool bConnected_{false};
    uint64_t pingId_{0};
};

#endif // WEBSOCKETCLIENT_H
