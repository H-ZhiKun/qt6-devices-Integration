/*
 * @Author: A炼 834022125@qq.com
 * @Date: 2023-06-15 14:56:55
 * @LastEditors: A炼 834022125@qq.com
 * @LastEditTime: 2023-06-16 16:21:15
 * @FilePath: \DeviceIntegration\src\camera\MyBaumerManage.h
 * @Description: 相机管理类，使用单例模式管理系统接入的相机
 * 
 * Copyright (c) 2023 by ${git_name_email}, All Rights Reserved. 
 */

#ifndef MYBAUMERMANAGE_H
#define MYBAUMERMANAGE_H

#include<QVector>
#include "Baumer/pcamera.h"
#include "MyBaumerCamera.h"

class MyBaumerCamera;
class MyBaumerManage{
public:
    // 获取单实例
    static MyBaumerManage* GetInstance();

    // 释放单实例，进程退出时调用
    static void deleteInstance();
    int updateCameraList();
    QVector<MyBaumerCamera*> cameraList;
    CSiSoGigE m_WorkSpace;

private:
    // 禁止外部构造和析构
    MyBaumerManage(){}
    ~MyBaumerManage(){}

    // 禁止外部拷贝和赋值
    MyBaumerManage(const MyBaumerManage &signal);
    const MyBaumerManage &operator=(const MyBaumerManage &signal);

private:
    // 唯一单实例对象指针
    static MyBaumerManage *g_pMyBaumerManage;
};
#endif // MYBAUMERMANAGE_H
