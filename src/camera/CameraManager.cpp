#include "CameraManager.h"
#include "Logger.h"
#include <QDebug>
#include <memory>
#include <mutex>

using namespace BGAPI2;

void BGAPI2CALL PnPEventHandler(void *callBackOwner, BGAPI2::Events::PnPEvent *pPnPEvent)
{
    if (NULL != pPnPEvent)
    {
        CameraManager *pManager = static_cast<CameraManager *>(callBackOwner);
        std::string EventTypeStr = ((pPnPEvent->GetPnPType() == 0) ? "removed" : "added");
        std::string SNNumber = pPnPEvent->GetSerialNumber().get();
        if (EventTypeStr == "removed")
        {
            // 回调删除出错相机
            LogInfo("camera callback remove: {}", SNNumber);
            pManager->removeCamera(SNNumber);
        }
        else if (EventTypeStr == "added")
        {
            LogInfo("camera callback add: {}", SNNumber);
        }
    }
}

CameraManager::CameraManager()
{
}

CameraManager::~CameraManager()
{
    stop();
}

void CameraManager::init(const YAML::Node &config)
{

    interfaceMAC_ = config["baumer"]["interface"]["mac"].as<std::string>();
    std::transform(interfaceMAC_.begin(), interfaceMAC_.end(), interfaceMAC_.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    std::replace(interfaceMAC_.begin(), interfaceMAC_.end(), '-', ':');
    interfaceMask_ = config["baumer"]["interface"]["mask"].as<std::string>();
    interfaceIp_ = config["baumer"]["interface"]["ip"].as<std::string>();

    nodeParams_ = config["baumer"]["camera"];

    for (const auto &group : nodeParams_)
    {
        const auto &key = group["display_window"].as<std::string>();
        mapCameraCounts_[key] = 0;
    }
    initTimer();
    // 以下是后台任务，不能阻塞主线程
    initializeBGAPI();
    searchCamera(); // 后台执行相机查找逻辑并添加
}

void CameraManager::cleanCameraCount()
{
    std::lock_guard lock(mtxCount_);
    for (auto &[key, count] : mapCameraCounts_)
    {
        count = 0;
    }
}

void CameraManager::resetCameraCount(const std::string &key, const uint32_t input)
{
    std::lock_guard lock(mtxCount_);
    auto &count = mapCameraCounts_.at(key);
    count = input;
}

void CameraManager::searchCamera()
{
    try
    {
        LogInfo("process in search camera.");
        BGAPI2::InterfaceList *interface_list = pSystem_->GetInterfaces();
        interface_list->Refresh(100);
        for (BGAPI2::InterfaceList::iterator ifc_iter = interface_list->begin(); ifc_iter != interface_list->end();
             ifc_iter++)
        {
            if (ifc_iter->second->IsOpen())
            {
                BGAPI2::DeviceList *device_list = ifc_iter->second->GetDevices();
                device_list->Refresh(100);
                for (BGAPI2::DeviceList::iterator device_iter = device_list->begin(); device_iter != device_list->end();
                     device_iter++)
                {
                    BGAPI2::NodeMap *pDeviceNodeMap = device_iter->second->GetNodeList();
                    std::string number = device_iter->second->GetSerialNumber().get();
                    if (forceIP(number, pDeviceNodeMap))
                    {
                        continue;
                    }
                    std::string status = device_iter->second->GetAccessStatus().get();
                    if (status == "RW" && !device_iter->second->IsOpen())
                    {
                        addCamera(number, device_iter->second);
                    }
                }
            }
        }
    }
    catch (BGAPI2::Exceptions::IException &ex)
    {
        LogError("Error Type: {}", ex.GetType().get());
        LogError("Error function: {}", ex.GetFunctionName().get());
        LogError("Error description: {}", ex.GetErrorDescription().get());
    }
}

void CameraManager::stop()
{
    bHold_ = false;
    timerSearch_->stop();
    mapCameras_.clear();
    deinitializeBGAPI();
}

void CameraManager::initTimer()
{
    timerSearch_ = new QTimer(this);
    QObject::connect(timerSearch_, &QTimer::timeout, [this]() { emit signalSearch(); });
    timerSearch_->setInterval(5000);
    timerSearch_->start();
}

void CameraManager::captureImageStart()
{
    for (const auto &[key, cam] : mapCameras_)
    {
        auto mat = cam->getCurrentMat();
        if (!mat.empty())
        {
            std::lock_guard lock(mtxCount_);
            if (!bHold_)
                return;
            auto &count = mapCameraCounts_.at(key);
            ++count;
            emit signalImgGet(key, count, mat);
        }
    }
}

void CameraManager::initializeBGAPI()
{
    try
    {
        BGAPI2::SystemList *system_list = BGAPI2::SystemList::GetInstance();
        system_list->Refresh();
        for (BGAPI2::SystemList::iterator sys_iter = system_list->begin(); sys_iter != system_list->end(); sys_iter++)
        {
            BGAPI2::System *system_pointer = sys_iter->second;
            BGAPI2::String tl_type = system_pointer->GetTLType();
            if (tl_type != "GEV")
            {
                continue;
            }
            system_pointer->Open();
            pSystem_ = system_pointer;
            BGAPI2::InterfaceList *interface_list = system_pointer->GetInterfaces();
            interface_list->Refresh(100);
            for (BGAPI2::InterfaceList::iterator ifc_iter = interface_list->begin(); ifc_iter != interface_list->end();
                 ifc_iter++)
            {
                ifc_iter->second->Open();
                std::string name = ifc_iter->second->GetDisplayName().get();
                std::string ip = ifc_iter->second->GetNode("GevInterfaceSubnetIPAddress")->GetValue().get();
                std::string mac = ifc_iter->second->GetNode("GevInterfaceMACAddress")->GetValue().get();
                std::string mask = ifc_iter->second->GetNode("GevInterfaceSubnetMask")->GetValue().get();
                // 为相机对应数据流注册掉线触发事件
                if (mac != interfaceMAC_)
                {
                    LogInfo("ignore {}, interface mac not matched.", mac);
                    ifc_iter->second->Close();
                    continue;
                }
                else if (ip != interfaceIp_)
                {
                    LogInfo("ignore ip {}, interface ip not matched.", ip);
                    ifc_iter->second->Close();
                    continue;
                }
                ifc_iter->second->RegisterPnPEvent(BGAPI2::Events::EVENTMODE_EVENT_HANDLER);
                ifc_iter->second->RegisterPnPEventHandler(this, (Events::PnPEventHandler)&PnPEventHandler);
                u32InterfaceIp_ = ifc_iter->second->GetNode("GevInterfaceSubnetIPAddress")->GetInt();
                u32InterfaceMask_ = ifc_iter->second->GetNode("GevInterfaceSubnetMask")->GetInt();
                LogInfo("interface founded: ");
                LogInfo("founded ip: {}", ip);
                LogInfo("founded MAC: {}", mac);
                LogInfo("founded mask: {}", mask);
            }
        }
    }
    catch (BGAPI2::Exceptions::IException &ex)
    {
        LogError("Error Type: {}", ex.GetType().get());
        LogError("Error function: {}", ex.GetFunctionName().get());
        LogError("Error description: {}", ex.GetErrorDescription().get());
    }
}

void CameraManager::deinitializeBGAPI()
{
    try
    {
        if (pSystem_ != nullptr)
        {
            BGAPI2::InterfaceList *interface_list = pSystem_->GetInterfaces();
            interface_list->Refresh(100);
            for (BGAPI2::InterfaceList::iterator ifc_iter = interface_list->begin(); ifc_iter != interface_list->end();
                 ifc_iter++)
            {
                if (ifc_iter->second->IsOpen())
                {
                    ifc_iter->second->UnregisterPnPEvent();
                    ifc_iter->second->Close();
                }
            }
            pSystem_->Close();
        }
        BGAPI2::SystemList::ReleaseInstance();
    }
    catch (BGAPI2::Exceptions::IException &ex)
    {
        LogError("Error Type: {}", ex.GetType().get());
        LogError("Error function: {}", ex.GetFunctionName().get());
        LogError("Error description: {}", ex.GetErrorDescription().get());
    }
}

bool CameraManager::addCamera(const std::string &snNumber, BGAPI2::Device *dev)
{
    bool ret = false;
    uint8_t index = 0;
    for (const auto &group : nodeParams_)
    {
        if (group["sn_number"].as<std::string>() == snNumber)
        {
            std::lock_guard lock(mtxCamera_);
            auto camera_obj = std::make_unique<BaumerCamera>(dev, group);
            if (!camera_obj->getInitialized())
            {
                return false;
            }
            mapCameras_[group["display_window"].as<std::string>()] = std::move(camera_obj);
            BGAPI2::NodeMap *node = dev->GetNodeList();
            LogInfo("sn={},ip={}, add success.", snNumber, node->GetNode("GevDeviceIPAddress")->GetValue().get());
            ret = true;
            break;
        }
    }
    return ret;
}

bool CameraManager::forceIP(const std::string &snNumber, BGAPI2::NodeMap *nodeMap)
{
    bool ret = false;
    uint8_t index = 0;
    for (; index < nodeParams_.size(); index++)
    {
        if (nodeParams_[index]["sn_number"].as<std::string>() == snNumber)
        {
            try
            {
                std::string subnetIp = nodeMap->GetNode("GevDeviceIPAddress")->GetValue().get();
                std::string subnetMask = nodeMap->GetNode("GevDeviceSubnetMask")->GetValue().get();
                std::string ip = nodeParams_[index]["ip"].as<std::string>();
                std::string mask = nodeParams_[index]["mask"].as<std::string>();
                if (ip != subnetIp || mask != subnetMask)
                {
                    bo_int64 iDeviceMacAddress = nodeMap->GetNode("GevDeviceMACAddress")->GetInt();
                    nodeMap->GetNode("MACAddressNeededToForce")->SetInt(iDeviceMacAddress);
                    nodeMap->GetNode("ForcedIPAddress")->SetValue(ip.c_str());
                    nodeMap->GetNode("ForcedSubnetMask")->SetValue(mask.c_str());
                    nodeMap->GetNode("ForcedGateway")->SetInt(u32InterfaceMask_ & u32InterfaceIp_);
                    nodeMap->GetNode("ForceIP")->Execute();
                    ret = true;
                }
            }
            catch (BGAPI2::Exceptions::IException &ex)
            {
                qDebug() << "ExceptionType:    " << ex.GetType().get();
                qDebug() << "ErrorDescription: " << ex.GetErrorDescription().get();
                qDebug() << "in function:      " << ex.GetFunctionName().get();
            }
        }
    }
    return ret;
}

void CameraManager::removeCamera(const std::string &snNumber)
{
    for (const auto &group : nodeParams_)
    {
        if (group["sn_number"].as<std::string>() == snNumber)
        {
            std::lock_guard lock(mtxCamera_);
            auto iter = mapCameras_.find(group["display_window"].as<std::string>());
            if (iter != mapCameras_.end())
            {
                mapCameras_.erase(iter);
                LogInfo("sn number {}, remove success.", snNumber);
                return;
            }
        }
    }
}

std::vector<std::string> CameraManager::cameraState()
{
    std::vector<std::string> res;
    for (const auto &[key, camera] : mapCameras_)
    {
        res.push_back(key);
    }
    return res;
}

uint32_t CameraManager::getCameraCount(const std::string windName)
{
    return mapCameraCounts_[windName];
}