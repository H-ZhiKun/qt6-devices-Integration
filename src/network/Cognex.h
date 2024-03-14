#pragma once
#include "TCPClient.h"
#include <QTimer>
#include <atomic>
#include <cstdint>
#include <string>

class Cognex : public TCPClient
{
    Q_OBJECT
  public:
    explicit Cognex(QObject *parent = nullptr);
    void dealing(std::vector<unsigned char> buffer) override;
    virtual ~Cognex() noexcept;
    uint32_t getCount();
    void cleanCount();
    void resetCount(const uint32_t count);
  signals:
    void connected();
    void disconnected();
    void responseReceived(const QByteArray &response);
    void readQRCode(uint32_t, const std::string &);

  private:
    std::atomic<uint32_t> recvCount_{0}; // 采集计数
};