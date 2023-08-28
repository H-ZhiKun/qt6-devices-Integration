#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QEventLoop>
#include <QObject>
#include <QTcpSocket>
#include <QTimer>
#include <QVariantList>
#include <vector>

class TCPClient : public QObject
{
    Q_OBJECT

  public:
    explicit TCPClient(QObject *parent = nullptr);
    virtual ~TCPClient() noexcept;
    TCPClient(const TCPClient &) = delete;
    TCPClient &operator=(const TCPClient &) = delete;
    TCPClient(TCPClient &&) noexcept(true) = default;
    TCPClient &operator=(TCPClient &&) noexcept(true) = default;
    virtual void dealing(std::vector<unsigned char>) = 0; // 数据接收处理接口，纯虚接口，必须重写
    virtual void pingBehavior(){};                        // 子类继承实现具体心跳行为
    inline bool getConnect();
  public slots:
    void startClient(const QString &host, quint16 port);
    void connectToServer(const QString &host = "", quint16 port = 0);
    void sendData(const QByteArray &data);

  public:
    void cleanup();
    const char *invokeStartClient = "startClient"; // invoke函数调用参数(const QString& host，quint16 port)
    const char *invokeConnectToServer = "connectToServer"; // invoke函数调用参数(const QString& host，quint16 port)

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
    bool bconnect_ = false;
};

#endif // TCPCLIENT_H
