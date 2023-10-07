#include "BaumerManager.h"
#include "Logger.h"
#include "Utils.h"
#include "fmt/format.h"
#include <QDebug>
#include <chrono>
using namespace BGAPI2;

void BGAPI2CALL PnPEventHandler(void *callBackOwner, BGAPI2::Events::PnPEvent *pPnPEvent)
{
    if (NULL != pPnPEvent)
    {
        BaumerManager *pManager = static_cast<BaumerManager *>(callBackOwner);
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
            // pManager->searchCamera();
        }
    }
}

BaumerManager::BaumerManager()
{
}

BaumerManager::~BaumerManager()
{
    stop();
}

void BaumerManager::start(const YAML::Node &launchConfig)
{

    interfaceMAC_ = launchConfig["baumer"]["interface"]["mac"].as<std::string>();
    std::transform(interfaceMAC_.begin(), interfaceMAC_.end(), interfaceMAC_.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    std::replace(interfaceMAC_.begin(), interfaceMAC_.end(), '-', ':');
    interfaceMask_ = launchConfig["baumer"]["interface"]["mask"].as<std::string>();
    interfaceIp_ = launchConfig["baumer"]["interface"]["ip"].as<std::string>();

    nodeParams_ = launchConfig["baumer"]["camera"];
    lvCameras_.resize(launchConfig["baumer"]["camera"].size());
    initializeBGAPI();
    searchCamera();
}

void BaumerManager::searchCamera()
{
    thSearch_ = std::thread([this] {
        while (bHold_.load(std::memory_order_acquire))
        {
            try
            {
                BGAPI2::InterfaceList *interface_list = pSystem_->GetInterfaces();
                interface_list->Refresh(100);
                for (BGAPI2::InterfaceList::iterator ifc_iter = interface_list->begin();
                     ifc_iter != interface_list->end(); ifc_iter++)
                {
                    if (ifc_iter->second->IsOpen())
                    {
                        BGAPI2::DeviceList *device_list = ifc_iter->second->GetDevices();
                        device_list->Refresh(100);
                        for (BGAPI2::DeviceList::iterator device_iter = device_list->begin();
                             device_iter != device_list->end(); device_iter++)
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
            std::this_thread::sleep_for(std::chrono::seconds(5));
        }
    });
}

void BaumerManager::stop()
{
    bHold_.store(false, std::memory_order_release);
    thSearch_.join();
    if (lvCameras_.size() > 0)
    {
        std::lock_guard lock(mtxCamera_);
        for (auto &camera : lvCameras_)
        {
            delete camera;
            camera = nullptr;
        }
        lvCameras_.clear();
    }
    deinitializeBGAPI();
}

cv::Mat BaumerManager::getCamaeraMat(uint8_t windId)
{
    std::lock_guard lock(mtxCamera_);
    if (lvCameras_[windId])
    {
        return lvCameras_[windId]->getCurrentMat();
    }
    return {};
};

void BaumerManager::initializeBGAPI()
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

void BaumerManager::deinitializeBGAPI()
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

bool BaumerManager::addCamera(const std::string &snNumber, BGAPI2::Device *dev)
{
    bool ret = false;
    uint8_t index = 0;
    for (; index < nodeParams_.size(); index++)
    {
        if (nodeParams_[index]["sn_number"].as<std::string>() == snNumber)
        {
            std::lock_guard lock(mtxCamera_);
            Camera *camera_obj = new Camera(dev, nodeParams_[index]);
            if (!camera_obj->getInitialized())
            {
                delete camera_obj;
                camera_obj = nullptr;
                return false;
            }
            lvCameras_[index] = camera_obj;
            BGAPI2::NodeMap *node = dev->GetNodeList();
            LogInfo("sn={},ip={}, add success.", snNumber, node->GetNode("GevDeviceIPAddress")->GetValue().get());
            ret = true;
            break;
        }
    }
    return ret;
}

bool BaumerManager::forceIP(const std::string &snNumber, BGAPI2::NodeMap *nodeMap)
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
                std::cout << "ExceptionType:    " << ex.GetType() << std::endl;
                std::cout << "ErrorDescription: " << ex.GetErrorDescription() << std::endl;
                std::cout << "in function:      " << ex.GetFunctionName() << std::endl;
            }
        }
    }
    return ret;
}

void BaumerManager::removeCamera(const std::string &snNumber)
{
    for (uint8_t index = 0; index < nodeParams_.size(); index++)
    {
        if (nodeParams_[index]["sn_number"].as<std::string>() == snNumber)
        {
            std::lock_guard lock(mtxCamera_);
            delete lvCameras_[index];
            lvCameras_[index] = nullptr;
            LogInfo("sn number {}, remove success.", snNumber);
        }
    }
}

void BaumerManager::saveConfig(YAML::Node &launchConfig)
{
    // for (uint8_t index = 0; index < lvParams_.size(); index++)
    // {
    //     auto curItem = launchConfig["baumer"]["paramters"][index];
    //     const auto &params = lvParams_[index];
    //     curItem["trigger_mode"] = params["trigger_mode"].asUInt();
    //     curItem["expose"] = params["expose"].asDouble();
    //     curItem["gain"] = params["gain"].asUInt();
    //     curItem["width"] = params["width"].asUInt();
    //     curItem["height"] = params["height"].asUInt();
    //     curItem["offset_x"] = params["offset_x"].asUInt();
    //     curItem["offset_y"] = params["offset_y"].asUInt();
    //     launchConfig["baumer"]["paramters"][index] = curItem;
    // }
}

std::vector<uint8_t> BaumerManager::cameraState()
{
    std::vector<uint8_t> res;
    for (uint8_t index = 0; index < lvCameras_.size(); index++)
    {
        if (lvCameras_[index])
        {
            res.push_back(index);
        }
    }
    return res;
}

bool BaumerManager::setCamera(uint8_t number, const Json::Value &param, std::string &des)
{
    bool ret = true;
    Camera *pCamera = nullptr;
    if (number < lvCameras_.size())
    {
        std::lock_guard lock(mtxCamera_);
        pCamera = lvCameras_[number];
    }
    if (pCamera == nullptr)
    {
        des = "camera init error.";
        return false;
    }
    pCamera->stopCollect();
    for (auto it = param.begin(); it != param.end(); ++it)
    {
        const std::string &key = it.key().asString();
        const std::string &strValue = it->asString();
        uint16_t value = 0;
        if (key == "expose")
        {
            double temp = Utils::anyFromString<double>(strValue);
            value = temp * 1000.0;
        }
        else
        {
            value = Utils::anyFromString<uint64_t>(strValue);
        }
        if (!pCamera->setParams(key, value))
        {
            LogError("failed camera param key = {}, camera value = {}", key, value);
            des += key + " set error;";
        }
    }
    pCamera->startCollect();
    ret = des.empty();
    return ret;
}

Json::Value BaumerManager::getCamera(uint8_t number)
{
    // Json::Value ret = lvParams_[number];
    // Json::Value temp;
    // if (number < lvCameras_.size())
    // {
    //     std::lock_guard lock(mtxCamera_);
    //     if (lvCameras_[number])
    //     {
    //         temp = lvCameras_[number]->getROParams();
    //     }
    //     else
    //     {
    //         return {};
    //     }
    // }
    // for (const auto &key : temp.getMemberNames())
    // {
    //     ret[key] = temp[key];
    // }
    return {};
}
