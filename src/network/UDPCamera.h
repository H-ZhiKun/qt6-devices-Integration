#pragma once
#include <QThread>
#include <QTimer>
#include <atomic>
#include <qobject.h>
#include <qtimer.h>
#include <qtypes.h>
#include <qudpsocket.h>
#include <unordered_map>

enum class UDPCmdType
{
    exposure = 0,
    gain,
    udpOn,
    udpOff
};

class UDPCamera : public QObject
{
    Q_OBJECT

  public:
    explicit UDPCamera(QObject *parent = nullptr);
    ~UDPCamera();
    void start();
    void triggerImage();
    void processDatagrams();

  protected:
    void processReceiveData(quint32 addr, const QByteArray &data);
  signals:
    void packetAssembled(QString windName, QByteArray packet);

  private:
    std::atomic_bool bTriggerImage_ = false;
    quint16 triggerTarget_ = 3;
    quint16 triggerCount_ = 0;
    std::atomic_bool bHold_ = true;
    QUdpSocket *udpSocket_;
    quint32 imgFrameSize_ = 1920 * 1080 * 1;
    QHostAddress senderAddress_;
    QByteArray udpOn_;
    QByteArray udpOff_;
    std::thread thRecv_;
    std::vector<quint32> vAddr_;
    quint16 indexAddr_ = 0;
    std::unordered_map<quint32, quint16> mapFrame_;
    std::unordered_map<quint32, quint16> mapPack_;
    std::unordered_map<quint32, QByteArray> mapBufferStore_;
};