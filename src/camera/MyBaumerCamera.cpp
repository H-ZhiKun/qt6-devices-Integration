#include "MyBaumerCamera.h"
#include <QDebug>
#include <functional>

MyBaumerCamera::MyBaumerCamera(QObject *parent)
        :QObject(parent){}

/**
 * @description: 相机数据获取的回调函数
 * @param {int} h 数据的高
 * @param {int} w 数据的宽
 * @param {void} *pData 回调的相机图像数据
 * @return {*}
 */
bool MyBaumerCamera::CameraProc0(int h,int w,void *pData)
{
    qDebug() << "m_width:" << m_width;
    m_width = 1920; //2448
    m_height = 1080; //2048

    qDebug() << "m_width:" << m_width;
    qDebug() << "m_height:" << m_height;
    /////////////数据显示

    if(bCopyShowImg)
    {
        memcpy(pShowBuff,(unsigned char*)pData, m_width * m_height);
        bCopyShowImg  = false;
    }

    //////////////数据存储
    if(bStartStore)
    {
        if(size < BUFF_SIZE)
        {
            memcpy(pBufferList[last], (unsigned char*)pData, m_width * m_height);
            InterlockedIncrement(&size);
            last++;
        }
        if(last == BUFF_SIZE)
        {
            last = 0;
        }
    }
    return true;
}

/**
 * @description: 打开相机，注册回调函数，开启线程
 * @return {*}
 */
bool MyBaumerCamera::OpenCamera()
{
    bool res = false;
    if(m_pCam->OpenCamera())
    {
        auto boundFunc = std::bind(&MyBaumerCamera::CameraProc0,this,
                                  std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
        m_pCam->RegisterEvent(boundFunc); //注册回调函数
        res = true;
        m_IsOpen = true;

        m_width      = m_pCam->GetWidth();
        m_height     = m_pCam->GetHeight();
        m_offsetX    = m_pCam->GetOffsetX();
        m_offsetY    = m_pCam->GetOffsetY();

        m_showW = 295;
        m_showH = 197;
        GetWindowSize();
        m_pCam->SetExposureTime(2000);

        m_storeThread = new MyThread(this);
        m_storeThread->start();
    }
    else
    {
        res = false;
    }

    return res;
}

/**
 * @description: 初始化指针等参数
 * @return {*}
 */
void MyBaumerCamera::Init_Parameter()
{

    pBuff = new unsigned char[IMG_W*IMG_H*2];
    memset(pBuff, 0, IMG_W*IMG_H*2);
    pShowBuff = new unsigned char[IMG_W*IMG_H];
    memset(pShowBuff, 0, IMG_W*IMG_H);
    m_imageProvider = new ImageProvider();

    for(int i = 0 ; i < BUFF_SIZE;i++)
    {
        pBufferList[i] = new unsigned char[IMG_W*IMG_H];
        memset(pBufferList[i] , 0, IMG_W*IMG_H);
    }

    m_IsStartGrab = false;
    m_IsOpen      = false;
    bStartStore   = false;

    size = 0;
    last = 0;
    front = 0;
}

/**
 * @description: 获取窗口的宽高位置信息
 * @return {*}
 */
void MyBaumerCamera::GetWindowSize()
{

    float fw1 = m_height * 1.0 / m_width;

    if(m_showH/fw1>m_showW)
    {
        m_W = m_showW;
        m_H = m_showW*fw1;
        m_X = 0;
        m_Y = (m_showH-m_H)/2;
    }
    else
    {
        m_H = m_showH;
        m_W = m_showH*1.0/fw1;
        m_X = (m_showW-m_W)/2;
        m_Y = 0;
    }
}

/**
 * @description: 重写时间循环，定期从回调函数中获取图像数据
 * @param {QTimerEvent} *e 获取图像数据的时间间隔
 * @return {*}
 */
void MyBaumerCamera::timerEvent(QTimerEvent *e)
{
    if(e->timerId() == m_timeID)
    {
        m_showW = 295;
        m_showH = 197;
        GetWindowSize();
        if(m_IsStartGrab==true && bCopyShowImg==false)
        {
            qDebug() << "timerEvent2";
            QImage qImage = QImage(pShowBuff, m_width, m_height, QImage::Format_Indexed8);
            qImage.setColorTable(vColorTable);
            QImage ImageScaled = qImage.scaled(m_W, m_H, Qt::KeepAspectRatio);
            //pixmap = QPixmap::fromImage(ImageScaled);
            m_imageProvider->updateImage(qImage);
            //std::cout << "m_W: "<< m_W << "  , m_H: " << m_H << std::endl;
            //std::cout << "m_width: "<< m_width << "  , m_height: " << m_height << std::endl;
            emit callQmlRefeshImg();

            bCopyShowImg = true;
        }
    }
}

/**
 * @description: 开启获取图像数据的线程
 * @return {*}
 */
void MyBaumerCamera::start(){
    m_width = m_pCam->GetWidth();
    m_height = m_pCam->GetHeight();
    m_pCam->StartCamera();
    bCopyShowImg = true;
    m_IsStartGrab = true;
    m_showW = 295;
    m_showH = 197;
    GetWindowSize();
    m_timeID = startTimer(100);
}

/**
 * @description: 析构函数，释放资源
 * @return {*}
 */
MyBaumerCamera::~MyBaumerCamera()
{
    m_pCam = nullptr;

    delete[] pBuff;
    pBuff  = nullptr;

    delete[] pShowBuff;
    pShowBuff = nullptr;

    delete m_imageProvider;
    m_imageProvider = nullptr;

    m_storeThread = nullptr;

    for (int i = 0; i < BUFF_SIZE; i++) {
        delete[] pBufferList[i];
    }

    // 释放数组指针
}
