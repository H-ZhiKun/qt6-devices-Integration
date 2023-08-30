#include "DeviceUpdate.h"

DeviceUpdate::DeviceUpdate(QObject *parent) : QObject(parent)
{
}

DeviceUpdate::~DeviceUpdate()
{
}

void DeviceUpdate::UpdateBottomMove(const uint8_t bottomNum)
{
    LogInfo("bottom {}: bottomMove signal, in {}", bottomNum, Utils::getCurrentTime(true));
    emit bottomMove(bottomNum);
}

void DeviceUpdate::UpdateReadQRCode(const uint8_t bottomNum)
{
    LogInfo("bottom {}: readQRCode signal, in {}", bottomNum, Utils::getCurrentTime(true));
    emit readQRCode(bottomNum);
}

void DeviceUpdate::UpdateLocatePhoto(const uint8_t winId, const uint64_t bottomNum)
{
    LogInfo("bottom {}: Photo locate image signal, in {}", bottomNum, Utils::getCurrentTime(true));
    emit locatePhoto(winId, bottomNum);
}

void DeviceUpdate::UpdateLocateCheckPhoto(const uint8_t winId, const uint64_t bottomNum)
{
    LogInfo("bottom {}: Photo locate check image signal, in {}", bottomNum, Utils::getCurrentTime(true));
    emit locateCheckPhoto(winId, bottomNum);
}

void DeviceUpdate::UpdateCodeLogistics(const uint8_t bottomNum)
{
    LogInfo("bottom {}: code Logistics signal, in {}", bottomNum, Utils::getCurrentTime(true));
    emit codeLogistics(bottomNum);
}

void DeviceUpdate::UpdateCodeCheck(const uint8_t winId, const uint64_t bottomNum)
{
    LogInfo("bottom {}: photo code check signal, in {}", bottomNum, Utils::getCurrentTime(true));
    emit codeCheck(winId, bottomNum);
}

void DeviceUpdate::UpdateCodeSerch(const uint8_t bottomNum)
{
    LogInfo("bottom {}: code serch signal, in {}", bottomNum, Utils::getCurrentTime(true));
    emit codeSerch(bottomNum);
}
