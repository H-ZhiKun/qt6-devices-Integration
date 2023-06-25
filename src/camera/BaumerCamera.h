/*
 * @Author: A炼 834022125@qq.com
 * @Date: 2023-06-15 14:56:55
 * @LastEditors: A炼 834022125@qq.com
 * @LastEditTime: 2023-06-25 16:37:03
 * @FilePath: \DeviceIntegration\src\camera\BaumerCamera.h
 * @Description: 单个相机的管理类，包括开启关闭和获取图像
 * 
 * Copyright (c) 2023 by ${git_name_email}, All Rights Reserved. 
 */
#ifndef BAUMERCAMERA_H
#define BAUMERCAMERA_H
//#include <QMainWindow>
#include <iostream>
#include "string.h"
#include <QThread>
#include <QString>
#include <QMessageBox>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QTimerEvent>
#include <QMessageBox>
#include <QFileDialog>
#include "BaumerManage.h"
#include "Windows.h"
#include "Siso/sisoIo.h"
#include "Mythread.h"
#include "AppFramework.h"
#include "ImageProvider.h"
#define BUFF_SIZE 200

class MyThread;


class BaumerCamera: public QObject
{
    Q_OBJECT
public:
    explicit BaumerCamera(QObject *parent = 0);
    ~BaumerCamera();
    void Init_Parameter();         //初始化参数
    bool OpenCamera();             //打开相机
    void GetImageBuff();           //获取图像数据
    void GetWindowSize();
    void start();
    bool CameraProc0(int h, int w, void *pData);

    inline ImageProvider *getImageProvider() { return m_imageProvider; }

    QString getCameraId()const{return cameraId;}
    void setCameraId(String cId){cameraId = cId;}

    volatile LONG getSize()const{return size;}

    bool getIsOpen()const{return m_IsOpen;}
    void setIsOpen(bool flag){m_IsOpen = flag;}

    int getWidth()const{return m_width;}
    void setWidth(int width){m_width = width;}

    int getHeight()const{return m_height;}
    void setHeight(int height){m_height = height;}

    int getOffsetX()const{return m_offsetX;}
    void setOffsetX(int offsetX){m_offsetX = offsetX;}

    int getOffsetY()const{return m_offsetY;}
    void setOffsetY(int offsetY){m_offsetY = offsetY;}

    int getFront()const{return front;}
    void setFront(int num){front = num;}

    int getLast()const{return last;}
    void setLast(int num){last = num;}

    int getmImgCount()const{return mImgCount;}
    void setmImgCount(int num){mImgCount = num;}

    QString getStrPath()const{return strPath;}
    void setStrPath(QString str){strPath = str;}

    pCamera                 *m_pCam;                //相机类指针
    unsigned char           *pBuff;                 //图像原始数据
    unsigned char           *pShowBuff;             //图像显示数据
    unsigned char           *pBufferList[BUFF_SIZE];//存储缓冲区

private:   
    bool                     m_IsStartGrab;         //是否开始采集
    bool                     m_IsOpen;              //是否打开相机
    int                      m_width;               //图像宽
    int                      m_height;              //图像高
    int                      m_offsetX;
    int                      m_offsetY;
    bool                     bStartStore;
    int                      mImgCount;
    MyThread                *m_storeThread;//数据存储线程
    QString                  strPath;
    volatile LONG            size;
    int                      front;
    int                      last;
    int                      m_timeID;
    int                      m_W;
    int                      m_H;
    int                      m_X;
    int                      m_Y;
    float                    fw;
    int                      m_showW;
    int                      m_showH;
    bool                     bCopyShowImg = false;
    QString                  cameraId;
    QPixmap                  pixmap;
    QVector<QRgb>            vColorTable;
    ImageProvider            *m_imageProvider;

signals:
    void callQmlRefeshImg();
    
private slots:
    void timerEvent(QTimerEvent*e);
};


#endif // BAUMERCAMERA_H
