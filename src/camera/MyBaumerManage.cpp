/*
 * @Author: A炼 834022125@qq.com
 * @Date: 2023-06-15 14:56:55
 * @LastEditors: A炼 834022125@qq.com
 * @LastEditTime: 2023-06-21 09:49:11
 * @FilePath: \DeviceIntegration\src\camera\MyBaumerManage.cpp
 * @Description: 
 * 
 * Copyright (c) 2023 by ${git_name_email}, All Rights Reserved. 
 */
#include "MyBaumerManage.h"

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

MyBaumerManage::~MyBaumerManage(){
    for(int i = 0; i < cameraList.size(); i++){
        delete cameraList[i];
        cameraList[i] = nullptr;
    }
}