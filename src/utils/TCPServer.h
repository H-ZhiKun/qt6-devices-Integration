#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>
#include <QVariantList>
#include <vector>

class TCPServer : public QObject
{
    Q_OBJECT

  public:
    explicit TCPServer(QObject *parent = nullptr);
    virtual ~TCPServer() noexcept;
    TCPServer(const TCPServer &) = delete;
    TCPServer &operator=(const TCPServer &) = delete;
    virtual void dealing(QTcpSocket *client, std::vector<unsigned char>) = 0; // 数据接收处理接口，纯虚接口，必须重写
    virtual void clientConnected(QTcpSocket *client){};                       // 子类继承实现具体连接行为
    virtual void clientDisconnected(QTcpSocket *client){}; // 子类继承实现具体断开连接行为

  public slots:
    void startServer(quint16 port);
    void stopServer();
    bool sendData(QTcpSocket *client, const QByteArray &data);

  private slots:
    void onNewConnection();
    void onClientDisconnected();
    void onReadyRead();

  private:
    QString socketErrorToString(QAbstractSocket::SocketError socketError);

  private:
    QTcpServer *tcpServer = nullptr;
    std::vector<QTcpSocket *> clients;
    quint16 serverPort;
};

#endif // TCPSERVER_H
