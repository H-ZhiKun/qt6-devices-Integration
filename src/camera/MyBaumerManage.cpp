#include "MyBaumerManage.h"

MyBaumerManage *MyBaumerManage::GetInstance()
{
    return g_pMyBaumerManage;
}

void MyBaumerManage::deleteInstance()
{
    if (g_pMyBaumerManage)
        {
            delete g_pMyBaumerManage;
            g_pMyBaumerManage = nullptr;
    }
}

int MyBaumerManage::updateCameraList()
{
    int res = -1;
    cameraList.clear();
    m_WorkSpace.Clear_System();
    if(m_WorkSpace.Initialize_System())
    {
        int m_cameraCount = (int)(m_WorkSpace.m_vectorCameras.size());
        if(m_cameraCount <=0)
        {
            m_WorkSpace.Clear_System();
            res = 0;
        }
        else
        {
            for(int i = 0; i < m_cameraCount; i++){
                MyBaumerCamera *tempCamera = new MyBaumerCamera;
                tempCamera->m_pCam = m_WorkSpace.m_vectorCameras[i];
                tempCamera->setCameraId(tempCamera->m_pCam->m_sDeviceID);
                cameraList.push_back(tempCamera);
            }
            res = m_cameraCount;
        }
    }
    return res;
}
