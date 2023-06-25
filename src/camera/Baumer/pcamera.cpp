#include "pcamera.h"
#include <QMessageBox>
#include <QDebug>
#include <QMessageBox>
pCamera::pCamera()
{
    // nCount = 0;
    m_IsBuffNull = true;
    m_IsStart = false;
    pBuff = new unsigned char[IMG_H*IMG_W*2];
    memset(pBuff,0,IMG_H*IMG_W*2);
    
    
    m_thread = new MyThread2(this);
    m_thread->start();

    m_imageTimeStamps.QuadPart = 0;
    QueryPerformanceFrequency(&m_liFreq);
}
CSiSoGigE::CSiSoGigE()
{
    
}

CSiSoGigE::~CSiSoGigE()
{
    Clear_System();
}
bool CSiSoGigE::Initialize_System()
{
    m_vectorCameras.clear();
    try
    {
        m_pSysList = SystemList::GetInstance();
        m_pSysList->Refresh();
    }
    catch(BGAPI2::Exceptions::IException& ex)
    {
        BGAPI2::String str1 = ex.GetFunctionName();
    }
    
    int size = m_pSysList->size();
    for(SystemList::iterator sy = m_pSysList->begin();sy!=m_pSysList->end();sy++)
    {
        m_pSystem = sy->second;
        m_pSystem->Open();
        BGAPI2::InterfaceList*   m_pIfList;
        try
        {
            m_pIfList = m_pSystem->GetInterfaces();
            m_pIfList->Refresh(1000);
        }
        catch(BGAPI2::Exceptions::IException& ex)
        {
            BGAPI2::String str1 = ex.GetFunctionName();
        }
        
        
        bo_uint64 xx = m_pIfList->size();
        int index = 0;
        for (InterfaceList::iterator ifc = m_pIfList->begin(); ifc != m_pIfList->end(); ifc++)
        {
            ifc->second->Open();
            DeviceList *deviceList = ifc->second->GetDevices();
            deviceList->Refresh(1000);

            if(deviceList->size() > 0)
            {
                for(DeviceList::iterator dev = deviceList->begin(); dev != deviceList->end(); dev++)
                {
                    pCamera* pCam = NULL;
                    pCam = new pCamera();
                    pCam->m_pSystem=m_pSystem;
                    pCam->m_pInterface = ifc->second;
                    pCam->m_pDeviceList=deviceList;
                    pCam->m_sInterfaceID = ifc->first;
                    pCam->m_pDevice = dev->second;
                    pCam->m_sDeviceID=dev->first;
                    pCam->m_strSN = dev->second->GetSerialNumber();
                    
                    if(m_vectorCameras.size()==0)
                    {
                        m_vectorCameras.append(pCam);
                    }
                    else
                    {
                        for(int i = 0 ; i < m_vectorCameras.size();i++)
                        {
                            if(m_vectorCameras.at(i)->m_strSN!=dev->second->GetSerialNumber())
                            {
                                m_vectorCameras.append(pCam);
                            }
                        }
                    }
                }
            }
            else
            {
                ifc->second->Close();
            }
        }
    }
    
    //    m_pSystem = m_pSysList->begin()++->second;
    //    m_pSystem->Open();
    
    
    return true;
}
void CSiSoGigE::Clear_System()
{
    try
    {
        m_pSystem->Close();
        BGAPI2::SystemList::ReleaseInstance();
        m_vectorCameras.clear();
    }
    catch (BGAPI2::Exceptions::IException& ex)
    {
        BGAPI2::String str =  ex.GetFunctionName();
    }
}

MyThread2::MyThread2(pCamera *pWindow, QObject *paraent):
    QThread(paraent)
{
    pCam = pWindow;
    stopped1 = false;
}
void MyThread2::Stop()
{
    stopped1 = true;
}
void MyThread2::run()
{
    while(!stopped1)
    {
        if(pCam->m_IsStart)
        {
            pCam->m_pBufferFiled =  pCam->m_pDataStream->GetFilledBuffer(1000); //timeout 1000 msec
            if(pCam->m_pBufferFiled == NULL)
            {
                qDebug()<<"Error: Buffer Timeout after 1000 msec";
            }
            else if(pCam->m_pBufferFiled->GetIsIncomplete() == true)
            {
                qDebug()<<"Error: Image is incomplete";
                // queue buffer again
                pCam->m_pBufferFiled->QueueBuffer();
            }
            else
            {
                unsigned char* transformBuffer = nullptr;
                transformBuffer = (unsigned char *) pCam->m_pBufferFiled->GetMemPtr();
                pCam->m_ImageW =  pCam->m_pBufferFiled->GetWidth();
                pCam->m_ImageH =  pCam->m_pBufferFiled->GetHeight();
                qDebug() << "befor pCam->m_pfCameraProc" ;

                pCam->m_pfCameraProc(pCam->m_ImageH,pCam->m_ImageW,(unsigned char*)transformBuffer);
                qDebug() << "after pCam->m_pfCameraProc" ;
                pCam->m_pBufferFiled->QueueBuffer();
            }
        }
    }
}


void BGAPI2CALL BufferHandler( void * callBackOwner, Buffer * pBufferFilled )
{
    pCamera* pCam = (pCamera*)callBackOwner;
    try
    {
        if(pBufferFilled == NULL)
        {
            qDebug()<<"Error: Buffer Timeout after 1000 msec";
        }
        else if(pBufferFilled->GetIsIncomplete() == true)
        {
            qDebug()<<"Error: Image is incomplete";
            pBufferFilled->QueueBuffer();
        }
        else
        {

            QueryPerformanceCounter(&pCam->cur);
            pCam->m_curFps = (double)(pCam->m_liFreq.QuadPart)/(double)( pCam->cur.QuadPart-pCam->m_imageTimeStamps.QuadPart);
            pCam->m_imageTimeStamps =  pCam->cur;

            unsigned char* transformBuffer = nullptr;
            transformBuffer = (unsigned char *) pBufferFilled->GetMemPtr();
            pCam->m_ImageW =  pBufferFilled->GetWidth();
            pCam->m_ImageH =  pBufferFilled->GetHeight();

            pCam->m_pfCameraProc(pCam->m_ImageH,pCam->m_ImageW,(unsigned char*)transformBuffer);

            pBufferFilled->QueueBuffer();
        }
    }
    catch (BGAPI2::Exceptions::IException& ex)
    {
        // QString str;
        // str.Format(_T("ExceptionType:%s! ErrorDescription:%s in function:%s"),ex.GetType(),ex.GetErrorDescription(),ex.GetFunctionName());

    }
}

bool pCamera::OpenCamera()
{
    try
    {
        m_pDevice->Open();
        m_pDatastreamList = m_pDevice->GetDataStreams();
        m_pDatastreamList->Refresh();
        
        m_pDataStream = m_pDatastreamList->begin()->second;
        m_pDataStream->Open();
        
        m_pBufferList = m_pDataStream->GetBufferList();
        
        for(int i = 0;i < 4;i++)
        {
            m_pBuffer = new BGAPI2::Buffer();
            m_pBufferList->Add(m_pBuffer);
        }
        
        for (BGAPI2::BufferList::iterator bufIterator = m_pBufferList->begin(); bufIterator != m_pBufferList->end(); bufIterator++)
        {
            bufIterator->second->QueueBuffer();
        }

        // m_pDataStream->RegisterNewBufferEvent(BGAPI2::Events::EVENTMODE_EVENT_HANDLER);
        // m_pDataStream->RegisterNewBufferEventHandler(this,(Events::NewBufferEventHandler) &BufferHandler);
    }
    catch(BGAPI2::Exceptions::IException& ex)
    {
    }
    
    return true;
}
void pCamera::CloseCamera()
{
    try
    {
        if(m_pDevice->GetRemoteNodeList()->GetNodePresent("AcquisitionAbort"))
        {
            m_pDevice->GetRemoteNode("AcquisitionAbort")->Execute();
        }
        m_pDevice->GetRemoteNode("AcquisitionStop")->Execute();
    }
    catch (BGAPI2::Exceptions::IException& ex)
    {
        
    }
    
    //STOP DataStream acquisition
    try
    {
        m_pDataStream->StopAcquisition();
        m_pBufferList->DiscardAllBuffers();
    }
    catch (BGAPI2::Exceptions::IException& ex)
    {
        //CString str2;
        //str2.Format(_T("ExceptionType:%s! ErrorDescription:%s in function:%s"),ex.GetType(),ex.GetErrorDescription(),ex.GetFunctionName());
        
    }
    
    //Release buffers
    try
    {
        while( m_pBufferList->size() > 0)
        {
            m_pBuffer = m_pBufferList->begin()->second;
            m_pBufferList->RevokeBuffer(m_pBuffer);
            delete m_pBuffer;
        }
        m_pInterface->UnregisterPnPEvent();
        //        m_pDataStream->UnregisterNewBufferEvent();
        //        m_pDataStream->RegisterNewBufferEvent(BGAPI2::Events::EVENTMODE_UNREGISTERED);
        m_pDataStream->Close();
        m_pDevice->Close();
        m_pInterface->Close();
    }
    catch (BGAPI2::Exceptions::IException& ex)
    {
        // QString str3 = QString("ExceptionType:%1! ErrorDescription:%1 in function:%1").arg(ex.GetType()).arg(ex.GetErrorDescription()).arg(ex.GetFunctionName());
    }
}

void pCamera::StartCamera()
{
    if(m_pDevice!=NULL)
    {
        try
        {
            m_pDataStream->StartAcquisitionContinuous();
            qDebug() << "StartAcquisitionContinuous";
            m_pDevice->GetRemoteNode("AcquisitionStart")->Execute();
            qDebug() << "GetRemoteNode";
            m_IsStart = true;
        }
        catch(BGAPI2::Exceptions::IException& ex)
        {
            
        }
    }
}
void pCamera::StopCamera()
{
    if(m_pDevice!=NULL)
    {
        try
        {
            m_IsStart = false;
            m_pDevice->GetRemoteNode("AcquisitionStop")->Execute();
            m_pDataStream->StopAcquisition();
        }
        catch(BGAPI2::Exceptions::IException& ex)
        {
        }
    }
}
int pCamera::GetWidth()
{
    int width;
    width = m_pDevice->GetRemoteNode("Width")->GetInt();
    return width;
}
int pCamera::GetHeight()
{
    int height;
    height = m_pDevice->GetRemoteNode("Height")->GetInt();
    return height;
}
int pCamera::GetOffsetX()
{
    int x;
    x = m_pDevice->GetRemoteNode("OffsetX")->GetInt();
    return x;
}
int pCamera::GetOffsetY()
{
    int y;
    y = m_pDevice->GetRemoteNode("OffsetY")->GetInt();
    return y;
}

void pCamera::SetWidth(int width)
{
    m_pDevice->GetRemoteNode("Width")->SetInt(width);
}
void pCamera::SetHeight(int height)
{
    m_pDevice->GetRemoteNode("Height")->SetInt(height);
}

void pCamera::SetExposureTime(double m_exp)
{
    m_pDevice->GetRemoteNode("ExposureTime")->SetDouble(m_exp);
}
double pCamera::GetExposureTime()
{
    int m_exp;
    m_exp = m_pDevice->GetRemoteNode("ExposureTime")->GetDouble();
    return m_exp;
}
double pCamera::GetExposureTimeMin()
{
    double m_exp;
    m_exp = m_pDevice->GetRemoteNode("ExposureTime")->GetDoubleMin();
    return m_exp;
}
double pCamera::GetExposureTimeMax()
{
    double m_exp;
    m_exp = m_pDevice->GetRemoteNode("ExposureTime")->GetDoubleMax();
    return m_exp;
}
void pCamera::SetGain(double m_gain)
{
    double m_gainMax = m_pDevice->GetRemoteNode("Gain")->GetDoubleMax();
    double m_gainMin = m_pDevice->GetRemoteNode("Gain")->GetDoubleMin();
    if(m_gain <= m_gainMin)
    {
        m_gain = m_gainMin;
    }
    if(m_gain >= m_gainMax)
    {
        m_gain = m_gainMax;
    }
    m_pDevice->GetRemoteNode("Gain")->SetDouble(m_gain);
}
double pCamera::GetGain()
{
    double gain;
    gain = m_pDevice->GetRemoteNode("Gain")->GetDouble();
    QString str = QString::number(gain,'f',2);
    gain = str.toFloat();
    return gain;
}
double pCamera::GetGainMin()
{
    double gain;
    gain = m_pDevice->GetRemoteNode("Gain")->GetDoubleMin();
    QString str = QString::number(gain,'f',2);
    gain = str.toFloat();
    return gain;
}
double pCamera::GetGainMax()
{
    double gain;
    gain = m_pDevice->GetRemoteNode("Gain")->GetDoubleMax();
    QString str = QString::number(gain,'f',2);
    gain = str.toFloat();
    return gain;
}
void pCamera::SetTriggerMode(bool bTriggerMode)
{
    if(bTriggerMode)
    {
        m_pDevice->GetRemoteNode("TriggerMode")->SetString("On");
        bTriggerMode = true;
    }
    else
    {
        m_pDevice->GetRemoteNode("TriggerMode")->SetString("Off");
        bTriggerMode = false;
    }
}
void pCamera::SetTriggerSource(int TriggerSource)
{
    try
    {
        if(TriggerSource == 0)
        {
            m_pDevice->GetRemoteNode("TriggerSource")->SetString("All");
        }
        else if(TriggerSource == 1)
        {
            m_pDevice->GetRemoteNode("TriggerSource")->SetString("Line0");
        }
        else if(TriggerSource == 2)
        {
            m_pDevice->GetRemoteNode("TriggerSource")->SetString("Software");
        }
        else
        {
            m_pDevice->GetRemoteNode("TriggerSource")->SetString("Off");
        }
    }
    catch(BGAPI2::Exceptions::IException& ex)
    {
        qDebug()<<"error111:"<<ex.GetFunctionName();
        ReleaseCamera();
        
    }
    
    
}
void pCamera::DoSoftwareTrigger()
{
    m_pDevice->GetRemoteNode("TriggerSoftware")->Execute();
}

float pCamera::GetCameraFps()
{
    float fps = m_pDataStream->GetNodeList()->GetNode("Throughput")->GetDouble();
    return fps;
}
void pCamera::AcquisitionFrameRateEnable(bool b)
{
    if(b)
    {
        m_pDevice->GetRemoteNode("AcquisitionFrameRateEnable")->SetBool(true);
    }
    else
    {
        m_pDevice->GetRemoteNode("AcquisitionFrameRateEnable")->SetBool(false);
    }
}

void pCamera::SetAcqusitionFrameRate(float fps)
{
    m_pDevice->GetRemoteNode("AcquisitionFrameRate")->SetDouble(fps);
}

void pCamera::SetPixelFormat(int pixel)
{
    if(pixel == 0)
    {
        m_pDevice->GetRemoteNode("PixelFormat")->SetString("Mono8");
    }
    else if(pixel ==1)
    {
        m_pDevice->GetRemoteNode("PixelFormat")->SetString("Mono12");
    }
}
void pCamera::ReInitCamera()
{
    
    if(m_pDeviceList != NULL)
    {
        m_pDeviceList->Refresh(100);
        m_pDevice=(*m_pDeviceList)[m_sDeviceID];
        m_pDevice->Open();
        m_pDatastreamList = m_pDevice->GetDataStreams();
        m_pDatastreamList->Refresh();
        m_pDataStream = m_pDatastreamList->begin()->second;
        m_pDataStream->Open();
        m_pBufferList = m_pDataStream->GetBufferList();
        for(int i = 0;i < 4;i++)
        {
            m_pBuffer = new BGAPI2::Buffer();
            m_pBufferList->Add(m_pBuffer);
        }
        for (BGAPI2::BufferList::iterator bufIterator = m_pBufferList->begin(); bufIterator != m_pBufferList->end(); bufIterator++)
        {
            bufIterator->second->QueueBuffer();
        }
    }
    StartCamera();
}

void pCamera::SetShortExposureTimeEnable(bool bVal)
{
    if(m_pDevice->GetRemoteNodeList()->GetNodePresent("ShortExposureTimeEnable"))
    {
        m_pDevice->GetRemoteNode("ShortExposureTimeEnable")->SetBool(bVal);
    }
}

bool pCamera::GetShortExposureTimeEnable()
{
    bool res =  m_pDevice->GetRemoteNode("ShortExposureTimeEnable")->GetBool();
    return res;
}

void pCamera::SetUserSet()
{
    m_pDevice->GetRemoteNode("UserSetSelector")->SetString("UserSet1");
    m_pDevice->GetRemoteNode("UserSetSave")->Execute();
    Sleep(1000);
    m_pDevice->GetRemoteNode("UserSetDefault")->SetString("UserSet1");
}

void pCamera::RegisterEvent(const std::function<bool(int, int, void*)>& fun)
{
    unsigned char* transformBuffer;
    m_pfCameraProc = fun;
}

void pCamera::ReleaseCamera()
{
    if(m_pDataStream != NULL)
    {
        m_pDataStream->StopAcquisition();
        m_pBufferList->DiscardAllBuffers();
        while(m_pBufferList->size()>0)
        {
            m_pBuffer = m_pBufferList->begin()->second;
            m_pBufferList->RevokeBuffer(m_pBuffer);
            delete m_pBuffer;
        }
        m_pDataStream->UnregisterNewBufferEvent();
        m_pDataStream->RegisterNewBufferEvent(BGAPI2::Events::EVENTMODE_UNREGISTERED);
        m_pDataStream->Close();
    }
    if(this->m_pDevice != NULL)
    {
        this->m_pDevice->Close();
        m_pDeviceList->Refresh(100);
    }
}

void pCamera::SetROIW(int w)
{
    int m_minwidth = m_pDevice->GetRemoteNode("Width")->GetIntMin();
    int m_maxwidth = m_pDevice->GetRemoteNode("Width")->GetIntMax();
    if(w <=m_minwidth)
        w = m_minwidth;
    if(w>=m_maxwidth)
        w = m_maxwidth;
    m_pDevice->GetRemoteNode("Width")->SetInt(w);
}
void pCamera::SetROIH(int h)
{
    int m_minheight = m_pDevice->GetRemoteNode("Height")->GetIntMin();
    int m_maxheight = m_pDevice->GetRemoteNode("Height")->GetIntMax();
    if(h <=m_minheight)
        h = m_minheight;
    if(h>=m_maxheight)
        h = m_maxheight;
    m_pDevice->GetRemoteNode("Height")->SetInt(h);
}
void pCamera::SetROIX(int x)
{
    int x_min = m_pDevice->GetRemoteNode("OffsetX")->GetIntMin();
    int x_max = m_pDevice->GetRemoteNode("OffsetX")->GetIntMax();
    
    if(x < x_min)
    {
        x = x_min;
    }
    if(x > x_max)
    {
        x = x_max;
    }
    m_pDevice->GetRemoteNode("OffsetX")->SetInt(x);
}
void pCamera::SetROIY(int y)
{
    int y_min = m_pDevice->GetRemoteNode("OffsetY")->GetIntMin();
    int y_max = m_pDevice->GetRemoteNode("OffsetY")->GetIntMax();
    
    if(y < y_min)
    {
        y = y_min;
    }
    if(y > y_max)
    {
        y = y_max;
    }
    m_pDevice->GetRemoteNode("OffsetY")->SetInt(y);
}

void pCamera::DoWhiteBalance()
{
    m_pDevice->GetRemoteNode("BalanceWhiteAuto")->SetString("Once");
    
}

int pCamera::GetTriggerDelay()
{
    int delay;
    delay = m_pDevice->GetRemoteNode("TriggerDelay")->GetInt();
    QString str = QString("%1").arg(delay);
    delay = str.toInt();
    return delay;
}
void pCamera::SetTriggerDelay(int delay)
{
    m_pDevice->GetRemoteNode("TriggerDelay")->SetInt(delay);
}

bool pCamera::GetImageBuff(unsigned char *buff)
{
    if(m_IsBuffNull)
        return false;
    memcpy(buff,pBuff,m_ImageH*m_ImageW);
    m_IsBuffNull=true;
    return m_IsBuffNull;
}
