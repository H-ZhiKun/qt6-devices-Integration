#include "DeviceUpdate.h"

DeviceUpdate::DeviceUpdate(QObject *parent) : QObject(parent)
{
}

DeviceUpdate::~DeviceUpdate()
{
}

void DeviceUpdate::UpdateBottomMove(int bottomNum)
{
    emit bottomMove(bottomNum);
}

void DeviceUpdate::UpdateReadQRCode(int bottomNum)
{
    emit readQRCode(bottomNum);
}

void DeviceUpdate::UpdateLocatePhoto(int winId, int bottomNum)
{
    emit locatePhoto(winId, bottomNum);
}

void DeviceUpdate::UpdateLocateCheckPhoto(int winId, int bottomNum)
{
    emit locateCheckPhoto(winId, bottomNum);
}

void DeviceUpdate::UpdateCodeLogistics(int bottomNum)
{
    emit codeLogistics(bottomNum);
}

void DeviceUpdate::UpdateCodeCheck(int winId, int bottomNum)
{
    emit codeCheck(winId, bottomNum);
}

void DeviceUpdate::UpdateCodeSerch(int bottomNum)
{
    emit codeSerch(bottomNum);
}
