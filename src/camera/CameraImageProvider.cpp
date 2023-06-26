/*
 * @Author: A炼 834022125@qq.com
 * @Date: 2023-06-16 16:18:24
 * @LastEditors: A炼 834022125@qq.com
 * @LastEditTime: 2023-06-21 11:37:07
 * @FilePath: \DeviceIntegration\src\camera\CameraImageProvider.cpp
 * @Description: 
 * 
 * Copyright (c) 2023 by ${git_name_email}, All Rights Reserved. 
 */

#include "CameraImageProvider.h"

CameraImageProvider::CameraImageProvider() : QQuickImageProvider(QQuickImageProvider::Image)
{

}

void CameraImageProvider::updateImage(QImage &image)
{
    m_image = image;
}

QImage CameraImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    Q_UNUSED(id);
    Q_UNUSED(size);
    Q_UNUSED(requestedSize);
    return this->m_image;
}

QPixmap CameraImageProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    Q_UNUSED(id);
    Q_UNUSED(size);
    Q_UNUSED(requestedSize);
    return QPixmap::fromImage(this->m_image);
}