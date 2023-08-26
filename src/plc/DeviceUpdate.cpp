#include "DeviceUpdate.h"

DeviceUpdate::DeviceUpdate(QObject *parent) : QObject(parent)
{
}

DeviceUpdate::~DeviceUpdate()
{
}

void DeviceUpdate::UpdateBottomMove(int moved)
{
    emit bottomMove(moved);
}
