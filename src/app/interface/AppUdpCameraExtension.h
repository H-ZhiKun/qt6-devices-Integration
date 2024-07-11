#pragma once
#include "BaseProduct.h"
#include "WebManager.h"
#include <AppFramework.h>

namespace AppFrame
{
class AppUdpCameraExtension : public NonCopyable
{
  public:
    static void aftercameraTragger(const QString windName, QByteArray &&bMat);
};
} // namespace AppFrame