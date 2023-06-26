#pragma once
#include <QQuickImageProvider>

class CameraImageProvider : public QQuickImageProvider
{
public:
    CameraImageProvider();

    void updateImage(QImage &image);    // 更新图像

    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;

    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize) override;
private:
    QImage m_image;
};
