#pragma once
#include "Logger.h"
#include "Utils.h"
#include <QObject>
class DeviceUpdate : public QObject
{
    Q_OBJECT
  public:
    explicit DeviceUpdate(QObject *parent = nullptr);
    ~DeviceUpdate();
    void UpdateBottomMove(const uint8_t);
    void UpdateReadQRCode(const uint8_t);
    void UpdateLocatePhoto(const uint8_t, const uint64_t);
    void UpdateLocateCheckPhoto(const uint8_t, const uint64_t);
    void UpdateCodeLogistics(const uint8_t);
    void UpdateCodeCheck(const uint8_t, const uint64_t);
    void UpdateCodeSerch(const uint8_t);
  signals:
    void bottomMove(const uint8_t);                       // 瓶位移动信号
    void readQRCode(const uint8_t);                       // 二维码读取信号
    void locatePhoto(const uint8_t, const uint64_t);      // 定位拍照信号
    void locateCheckPhoto(const uint8_t, const uint64_t); // 定位复核拍照信号
    void codeLogistics(const uint8_t);                    // 打码信号
    void codeCheck(const uint8_t, const uint64_t);        // 打码复核信号
    void codeSerch(const uint8_t);                        // 喷码数据查找信号
};
