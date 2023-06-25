/*
 * @Author: A炼 834022125@qq.com
 * @Date: 2023-06-15 14:56:55
 * @LastEditors: A炼 834022125@qq.com
 * @LastEditTime: 2023-06-25 17:05:48
 * @FilePath: \DeviceIntegration\src\camera\BaumerManage.h
 * @Description: 相机管理类，使用单例模式管理系统接入的相机
 * 
 * Copyright (c) 2023 by ${git_name_email}, All Rights Reserved. 
 */

#ifndef BAUMERMANAGE_H
#define BAUMERMANAGE_H

#include<QVector>
#include "Baumer/pcamera.h"
#include "BaumerCamera.h"

class BaumerCamera;
class BaumerManage{
public:
    int updateCameraList();
    QVector<BaumerCamera*> cameraList;
    CSiSoGigE m_WorkSpace;
    BaumerManage(){}
    ~BaumerManage();
    QVector<ImageProvider*> imageProvider;

private:
    // 禁止外部拷贝和赋值
    BaumerManage(const BaumerManage &signal);
    const BaumerManage &operator=(const BaumerManage &signal);
};
#endif // BAUMERMANAGE_H
