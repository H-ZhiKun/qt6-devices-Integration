#pragma once
#include <QObject>

class DeviceUpdate : public QObject
{
    Q_OBJECT
  public:
    explicit DeviceUpdate(QObject *parent = nullptr);
    ~DeviceUpdate();
    void UpdateBottomMove(int);
  signals:
    void bottomMove(int);            // 瓶位移动信号
    void readQRCode(int);            // 二维码读取信号
    void locatePhoto(int, int);      // 定位拍照信号
    void locateCheckPhoto(int, int); // 定位复核拍照信号
    void codeLogistics(int);         // 打码信号
    void codeCheck(int, int);        // 打码复核信号
    void codeSerch(int);             // 喷码数据查找信号
};
