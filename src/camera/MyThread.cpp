/*
 * @Author: A炼 834022125@qq.com
 * @Date: 2023-06-15 14:56:55
 * @LastEditors: A炼 834022125@qq.com
 * @LastEditTime: 2023-06-19 15:31:37
 * @FilePath: \DeviceIntegration\src\camera\MyThread.cpp
 * @Description: 
 * 
 * Copyright (c) 2023 by ${git_name_email}, All Rights Reserved. 
 */
#include "MyThread.h"

//数据存储线程
MyThread::MyThread(MyBaumerCamera *pCamera, QObject *paraent)
{
    pCameraDlg = pCamera;
    stopped1 = false;
}

void MyThread::run()
{
    while(!stopped1)
    {
        if(pCameraDlg->getSize()>0)
        {
            QString str = pCameraDlg->getStrPath() + "/"+QString("%1.bmp").arg(pCameraDlg->getmImgCount(), 5, 10, QChar('0'));
            QByteArray ba = str.toLatin1();
            IoWriteBmp(ba.data(), (unsigned char*)pCameraDlg->pBufferList[pCameraDlg->getFront()], pCameraDlg->getWidth(), pCameraDlg->getHeight(), 8);
            int imgCount = pCameraDlg->getmImgCount();
            pCameraDlg->setmImgCount(++imgCount);
            volatile LONG size = pCameraDlg->getSize();
            InterlockedDecrement(&size);
            int front = pCameraDlg->getFront();
            pCameraDlg->setFront(++front);
            if (pCameraDlg->getFront() == BUFF_SIZE )
            {
                pCameraDlg->setFront(0);
            }
        }
    }
}

MyThread::~MyThread(){
    delete pCameraDlg;
    pCameraDlg = nullptr;

}
