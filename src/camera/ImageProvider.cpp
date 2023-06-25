/*
 * @Author: A炼 834022125@qq.com
 * @Date: 2023-06-16 16:18:24
 * @LastEditors: A炼 834022125@qq.com
 * @LastEditTime: 2023-06-21 11:37:07
 * @FilePath: \DeviceIntegration\src\camera\imageprovider.cpp
 * @Description: 
 * 
 * Copyright (c) 2023 by ${git_name_email}, All Rights Reserved. 
 */

#include "ImageProvider.h"

ImageProvider::ImageProvider() : QQuickImageProvider(QQuickImageProvider::Image)
{

}

void ImageProvider::updateImage(QImage &image)
{
    m_image = image;
}

QImage ImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    Q_UNUSED(id);
    Q_UNUSED(size);
    Q_UNUSED(requestedSize);
    return this->m_image;
}

QPixmap ImageProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    Q_UNUSED(id);
    Q_UNUSED(size);
    Q_UNUSED(requestedSize);
    return QPixmap::fromImage(this->m_image);
}