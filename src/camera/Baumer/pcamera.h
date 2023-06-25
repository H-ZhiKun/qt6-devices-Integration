#ifndef PCAMERA_H
#define PCAMERA_H
#include "Baumer/bgapi2_genicam.hpp"
using namespace BGAPI2;
#include <QVector>
#include <QLabel>
#include <QTimerEvent>
#include <QThread>
#include "Windows.h"
#define CAMERANUM 20
#define IMG_W 2048
#define IMG_H 2048


typedef bool (__cdecl*pfCameraProc1)(int,int,void*);


class CSiSoGigE;
class MyThread2;
class pCamera
{
public:
    pCamera();
public:
    bool        OpenCamera();
    void        CloseCamera();
    void        StartCamera();
    void        StopCamera();

    void        SetExposureTime(double m_exp);
    double      GetExposureTime();
    double      GetExposureTimeMin();
    double      GetExposureTimeMax();

    int         GetTriggerDelay();
    void        SetGain(double m_gain);
    double      GetGain();
    double      GetGainMin();
    double      GetGainMax();

    float       GetCameraFps();
    void        AcquisitionFrameRateEnable(bool b);
    void        SetAcqusitionFrameRate(float fps);

    int         GetWidth();
    int         GetHeight();
    int         GetOffsetX();
    int         GetOffsetY();

    void        SetROIW(int w);
    void        SetROIH(int h);
    void        SetROIX(int x);
    void        SetROIY(int y);

    void        SetWidth(int width);
    void        SetHeight(int height);

    void        SetTriggerMode(bool bTriggerMode);
    void        SetTriggerSource(int TriggerSource);//TriggerSource = 0:外触发；TriggerSource = 1：软触发
    void        DoSoftwareTrigger();

    void        SetPixelFormat(int pixel);

    void        DoWhiteBalance();
    void        SetTriggerDelay(int delay);

    bool        GetImageBuff(unsigned char*buff);

    void        ReleaseCamera();
    void        ReInitCamera();

    void        SetShortExposureTimeEnable(bool bVal);
    bool        GetShortExposureTimeEnable();
    void        SetUserSet();

    void        RegisterEvent(const std::function<bool(int, int, void*)>& fun);   //注册回调函数（备用）



public:
    CSiSoGigE			     *m_pSiSoGigE;

    BGAPI2::System*           m_pSystem;
    BGAPI2::Interface*        m_pInterface;
    BGAPI2::String            m_sInterfaceID;
    //variables for Devicelist and device
    BGAPI2::DeviceList*       m_pDeviceList;
    BGAPI2::Device *          m_pDevice;
    BGAPI2::String            m_sDeviceID;

    //variables for Datastreamlist and datastream
    BGAPI2::DataStreamList*   m_pDatastreamList;
    BGAPI2::DataStream*       m_pDataStream;
    BGAPI2::String            m_sDataStreamID;

    //variables for Bufferlist and buffer
    BGAPI2::BufferList*       m_pBufferList;
    BGAPI2::Buffer*           m_pBuffer;
    BGAPI2::String            m_sBufferID;

    BGAPI2::Buffer*           m_pBufferFiled;

    QString                   m_strSN;


    int                      m_ImageW;
    int                      m_ImageH;
    int                      m_LabelW;
    int                      m_LabelH;
    QLabel                   *m_ImageLabel;

    MyThread2               *m_thread;
    std::function<bool(int, int, void*)>  m_pfCameraProc;

    bool                     m_IsStart;
    bool                     m_IsBuffNull;
    unsigned char           *pBuff;


    LARGE_INTEGER            m_imageTimeStamps;
    LARGE_INTEGER            m_liFreq;
    double                   m_curFps;
    LARGE_INTEGER            cur;

};

class CSiSoGigE
{
public:
    CSiSoGigE(void);
    ~CSiSoGigE(void);

public:
    QVector<pCamera*>m_vectorCameras;
    
    bool Initialize_System();
    void Clear_System();
public:
    BGAPI2::SystemList*      m_pSysList;
    BGAPI2::System*          m_pSystem;
};

class MyThread2:public QThread
{
public:
    MyThread2(pCamera *pWindow,QObject*paraent = 0);
    void Stop();
protected:
    void run();
public:
    pCamera *pCam;
private:
    volatile bool stopped1;
};

#endif // PCAMERA_H
