#pragma once
#include "Logger.h"
#include "TCPClient.h"
#include <QTimer>
#include <string>

class Cognex : public TCPClient
{
    Q_OBJECT
  public:
    explicit Cognex(QObject *parent = nullptr);
    void dealing(std::vector<unsigned char> buffer) override;
    // void pingBehavior() override;
    virtual ~Cognex() noexcept;

    // invoke函数调用
    QString scanOnce()
    {
        return "scanCode";
    }

  public slots:
    void scanCode();
    void scanStop();

  signals:
    void connected();
    void disconnected();
    void responseReceived(const QByteArray &response);
    void ReadQRCode(const std::string &);
};