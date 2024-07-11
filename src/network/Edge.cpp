#include "Edge.h"
#include "Logger.h"
#include "Utils.h"

Edge::Edge(QObject *parent) : TCPServer(parent)
{
}

Edge::~Edge() noexcept
{
}

void Edge::dealing(QTcpSocket *client, std::vector<unsigned char> data)
{
    std::string recvData(data.begin(), data.end());
    // 处理完整的消息
    Json::Value recvJson = Utils::stringToJson(recvData);
    if (!recvJson.empty())
    {
        LogInfo("Received edge {} json: {}", client->peerAddress().toString().toStdString(), recvData);
    }
    else
    {
        LogError("Received edge {} json empty, message: {}", client->peerAddress().toString().toStdString(), recvData);
    }
}

void Edge::clientConnected(QTcpSocket *client)
{
    QString clientIP = client->peerAddress().toString();
    if (clientIP.startsWith("::ffff:"))
    {
        clientIP = clientIP.mid(7);
    }
    mapClients.insert(clientIP, client);
    LogInfo("Client {} connected.", clientIP.toStdString());
    // 在这里添加其他处理逻辑，例如向客户端发送欢迎消息
}

void Edge::clientDisconnected(QTcpSocket *client)
{
    QString clientIP = client->peerAddress().toString();
    if (clientIP.startsWith("::ffff:"))
    {
        clientIP = clientIP.mid(7);
    }
    mapClients.remove(clientIP);
    clientBuffers.remove(client);
    LogInfo("Client {} disconnected.", clientIP.toStdString());
}