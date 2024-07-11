#include "UDPCamera.h"
#include <cstddef>
#include <qdatetime.h>
#include <qdebug.h>
#include <qhostaddress.h>
#include <qobject.h>
#include <qtimer.h>
#include <qtypes.h>
#include <qudpsocket.h>
#include <thread>

UDPCamera::UDPCamera(QObject *parent) : QObject(parent), udpSocket_(nullptr)
{
}

UDPCamera::~UDPCamera()
{
    bHold_ = false;
    if (thRecv_.joinable())
        thRecv_.join();
}

void UDPCamera::start()
{
    udpOn_ = QByteArray::fromHex("3B34561A");
    udpOff_ = QByteArray::fromHex("3B345645");
    for (size_t i = 2; i <= 6; i++)
    {
        QString ip = QString("192.168.0.") + QString().number(i);
        quint32 addr = QHostAddress(ip).toIPv4Address();
        mapFrame_[addr] = 0;
        mapPack_[addr] = 0;
        mapFrame_[addr] = 0;
        mapBufferStore_[addr] = QByteArray();
        vAddr_.emplace_back(addr);
    }
    udpSocket_ = new QUdpSocket(this);
    udpSocket_->bind(QHostAddress::Any,
                     1234); // Bind to any address and port 1234
    udpSocket_->setSocketOption(QAbstractSocket::ReceiveBufferSizeSocketOption, 10485760);
    auto bufferSize = udpSocket_->socketOption(QAbstractSocket::ReceiveBufferSizeSocketOption);
    qDebug() << "Receive buffer size: " << bufferSize.value<qint64>();
    // connect(udpSocket_, &QUdpSocket::readyRead, this, &UdpReceiverWorker::processPendingDatagrams);
    processDatagrams();
}
void UDPCamera::triggerImage()
{
    bTriggerImage_ = true;
}

void UDPCamera::processDatagrams()
{
    thRecv_ = std::thread([this] {
        while (bHold_)
        {
            if (udpSocket_->hasPendingDatagrams())
            {
                QByteArray data;
                data.resize(udpSocket_->pendingDatagramSize());
                auto size = udpSocket_->readDatagram(data.data(), data.size(), &senderAddress_);
                // 解析帧号和包号
                auto addr = senderAddress_.toIPv4Address();
                if (!bTriggerImage_ || addr != vAddr_[indexAddr_])
                {
                    udpSocket_->writeDatagram(udpOff_, senderAddress_, 1234);
                    continue;
                }
                processReceiveData(addr, data);
            }
            else
            {
                if (bTriggerImage_)
                {
                    udpSocket_->writeDatagram(udpOn_, QHostAddress(vAddr_[indexAddr_]), 1234);
                    udpSocket_->writeDatagram(udpOff_, QHostAddress(vAddr_[indexAddr_]), 1234);
                }
            }
        }
    });
}

void UDPCamera::processReceiveData(quint32 addr, const QByteArray &data)
{
    auto &lastFrame = mapFrame_.at(addr);
    auto &lastPack = mapPack_.at(addr);
    auto &store = mapBufferStore_.at(addr);
    quint16 frameNum = (static_cast<quint16>(static_cast<uint8_t>(data[0])) << 8) |
                       static_cast<quint16>(static_cast<uint8_t>(data[1]));
    quint16 packNum = (static_cast<quint16>(static_cast<uint8_t>(data[2])) << 8) |
                      static_cast<quint16>(static_cast<uint8_t>(data[3]));
    if (packNum == 0)
    {
        if (store.size() == imgFrameSize_)
        {
            if (addr == vAddr_[0])
            {
                triggerCount_++;
                if (triggerCount_ > triggerTarget_)
                {
                    bTriggerImage_ = false;
                    triggerCount_ = 0;
                    return;
                }
            }
            emit packetAssembled(senderAddress_.toString(), store);
            indexAddr_++;
            indexAddr_ = indexAddr_ % vAddr_.size();
        }
        store.clear();
        store.append(data.mid(4));
        lastFrame = frameNum;
        lastPack = packNum;
        return;
    }

    if (lastPack + 1 == packNum)
    {
        lastPack = packNum;
        store.append(data.mid(4));
    }
}