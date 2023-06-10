#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QEventLoop>
#include <QTcpSocket>
#include <QTimer>
#include <vector>
#include <QVariantList>

class TCPClient : public QObject
{
    Q_OBJECT

public:
    explicit TCPClient(QObject *parent = nullptr);
    virtual ~TCPClient() noexcept;
    virtual void dealing(std::vector<unsigned char>) = 0; // 数据接收处理接口，纯虚接口，必须重写
    virtual void pingBehavior(){};                        // 子类继承实现具体心跳行为
public slots:
    void startClient(const QString &host, quint16 port);
    void connectToServer(const QString &host = "", quint16 port = 0);
    void sendData(const QByteArray &data);
public:
    void cleanup();
    // 获取方法名称系列函数
    /**
     * @brief 函数调用参数(const QString& host，quint16 port)
     */
     QString getNameConnectToServer();
    /**
     * @brief 函数调用参数(const QString& host，quint16 port)
     */
     QString getNameStartClient();
private slots:
    void onConnected();
    void onDisconnected();
    void onError(QAbstractSocket::SocketError errCode);
    void onReadyRead();
    void reconnect();
    void ping();

private:
    void pingEnable(bool bEnable); // 是否开启心跳
    QString socketErrorToString(QAbstractSocket::SocketError socketError);

private:
    QTcpSocket *tcpSocket = nullptr;
    QTimer *reconnectTimer = nullptr;
    QTimer *pingHolder = nullptr;
    QString serverHost;
    quint16 serverPort;
};

#endif // TCPCLIENT_H
