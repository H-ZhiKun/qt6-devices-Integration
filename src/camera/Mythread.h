/*
 * @Author: A炼 834022125@qq.com
 * @Date: 2023-06-15 14:56:55
 * @LastEditors: A炼 834022125@qq.com
 * @LastEditTime: 2023-06-16 16:25:11
 * @FilePath: \DeviceIntegration\src\camera\MyThread.cpp
 * @Description: 数据存储线程
 * 
 * Copyright (c) 2023 by ${git_name_email}, All Rights Reserved. 
 */
#ifndef MYTHREAD_H
#define MYTHREAD_H

#include "MyBaumerCamera.h"

class MyBaumerCamera;
class MyThread: public QThread
{
public:
    MyThread(MyBaumerCamera *pCamera, QObject*paraent = 0);
    ~MyThread();
    void stop();
protected:
    void run();
public:
    MyBaumerCamera *pCameraDlg;
private:
    volatile bool stopped1;
};

#endif // MYTHREAD_H
