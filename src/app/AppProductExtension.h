#pragma once
#include "BaseProduct.h"
#include "WebManager.h"
#include <AppFramework.h>

namespace AppFrame
{
class AppProductionExtension : public NonCopyable
{
  public:
    static void afterProductIn(const uint32_t count);
    static void afterProductOut(const uint32_t count);
    static void afterCognexRecv(uint32_t number, const std::string &code);
    static void afterPermissionRecv(uint32_t, const std::string &code);
    static void afterCaputureImage(const std::string windName, uint32_t number, const cv::Mat &mat);
    static void afterALGORecv(const ALGOType, const uint32_t, const std::string &, const std::string &);
    static std::string tangleCount(const std::string &result, const BottleShape bottleShape, const SubType subType,
                                   const int tangelOffset);
};
} // namespace AppFrame