#ifndef EDGE_H
#define EDGE_H

#include "TCPServer.h"
#include <QByteArray>
#include <QObject>
#include <QTcpSocket>
#include <vector>

class Edge : public TCPServer
{
    Q_OBJECT

  public:
    explicit Edge(QObject *parent = nullptr);
    virtual ~Edge() noexcept override;

  protected:
    void dealing(QTcpSocket *client, std::vector<unsigned char> data) override;
    void clientConnected(QTcpSocket *client) override;
    void clientDisconnected(QTcpSocket *client) override;

  private:
    QMap<QTcpSocket *, QByteArray> clientBuffers; // 用于存储每个客户端的临时数据
    QMap<QString, QTcpSocket *> mapClients;
};

#endif // EDGE_H
