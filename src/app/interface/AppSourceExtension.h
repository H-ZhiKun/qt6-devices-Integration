#pragma once
#include "NonCopyable.h"
#include <QDateTime>

namespace AppFrame
{
enum class ResourcePath
{
    PathImgNormal = 0,
    PathImgNG
};
class AppSourceExtension : public NonCopyable
{
  private:
    inline static QString pathNormalImage_;
    inline static QString pathNGImage_;

  public:
    static void initFilePath(QString &&appPath);         // 初始化图片保存路径
    static QString getFilePath(const ResourcePath path); // 获取图片保存路径
    static void deleteImage(const uint16_t &mouthNormal, const uint16_t &mouthNG, QString &&appPath); // 图片超期删除
};
} // namespace AppFrame