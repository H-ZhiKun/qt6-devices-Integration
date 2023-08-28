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
            pManager->searchCamera();
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
    MACAddress = launchConfig["baumer"]["interface"].as<std::string>();
    std::transform(MACAddress.begin(), MACAddress.end(), MACAddress.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    std::replace(MACAddress.begin(), MACAddress.end(), '-', ':');
    lvParams_.resize(launchConfig["baumer"]["paramters"].size());
    lvSNNumber_.resize(launchConfig["baumer"]["paramters"].size());
    lvCameras_.resize(launchConfig["baumer"]["paramters"].size());
    for (const auto &item : launchConfig["baumer"]["paramters"])
    {
        uint8_t winNumber = item["display_window"].as<uint8_t>();
        Json::Value jsVal;
        jsVal["trigger_mode"] = item["trigger_mode"].as<uint16_t>();
        jsVal["expose"] = item["expose"].as<double>();
        jsVal["gain"] = item["gain"].as<uint16_t>();
        jsVal["width"] = item["width"].as<uint16_t>();
        jsVal["height"] = item["height"].as<uint16_t>();
        jsVal["offset_x"] = item["offset_x"].as<uint16_t>();
        jsVal["offset_y"] = item["offset_y"].as<uint16_t>();
        lvParams_[winNumber] = std::move(jsVal);
        lvSNNumber_[winNumber] = std::move(item["sn_number"].as<std::string>());
    }
    initializeBGAPI();
    searchCamera();
}

void BaumerManager::searchCamera()
{
    static bool bSearch = false;
    if (bSearch)
        return;
    bSearch = true;
    Utils::asyncTask([this] {
        while (true)
        {
            int count = 0;
            for (auto camera : lvCameras_)
            {
                if (camera)
                {
                    count++;
                }
            }
            if (count == lvCameras_.size())
            {
                break;
            }
            try
            {
                if (pInterface_)
                {
                    BGAPI2::DeviceList *device_list = pInterface_->GetDevices();
                    device_list->Refresh(100);
                    for (BGAPI2::DeviceList::iterator device_iter = device_list->begin();
                         device_iter != device_list->end(); device_iter++)
                    {
                        std::string number = device_iter->second->GetSerialNumber().get();
                        std::string status = device_iter->second->GetAccessStatus().get();
                        if (status == "RW" && !device_iter->second->IsOpen())
                        {
                            addCamera(number, device_iter->second);
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
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        bSearch = false;
    });
}

void BaumerManager::stop()
{
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

std::list<cv::Mat> BaumerManager::getImageBySN(uint8_t number)
{
    std::lock_guard lock(mtxCamera_);
    return lvCameras_[number]->getImage();
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
                if (mac != MACAddress)
                {
                    LogInfo("interface not matched: ");
                    LogInfo("ignore ip: {}", ip);
                    LogInfo("ignore MAC: {}", mac);
                    ifc_iter->second->Close();
                    continue;
                }
                ifc_iter->second->RegisterPnPEvent(BGAPI2::Events::EVENTMODE_EVENT_HANDLER);
                ifc_iter->second->RegisterPnPEventHandler(this, (Events::PnPEventHandler)&PnPEventHandler);
                LogInfo("interface founded: ");
                LogInfo("founded ip: {}", ip);
                LogInfo("founded MAC: {}", mac);
                pInterface_ = ifc_iter->second;
                break;
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
        if (pInterface_ != nullptr)
        {
            pInterface_->UnregisterPnPEvent();
            pInterface_->Close();
        }
        if (pSystem_ != nullptr)
        {
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
    for (; index < lvSNNumber_.size(); index++)
    {
        if (lvSNNumber_[index] == snNumber)
        {
            if (lvCameras_[index])
            {
                return true;
            }
            std::lock_guard lock(mtxCamera_);
            Camera *camera_obj = new Camera(dev);
            if (!camera_obj->getInitialized())
            {
                delete camera_obj;
                camera_obj = nullptr;
                return false;
            }
            lvCameras_[index] = camera_obj;
            LogInfo("sn number {}, add success.", snNumber);
            ret = true;
            break;
        }
    }
    if (ret)
    {
        std::string des;
        setCamera(lvParams_[index], des);
    }
    return ret;
}

void BaumerManager::removeCamera(const std::string &snNumber)
{
    for (uint8_t index = 0; index < lvSNNumber_.size(); index++)
    {
        if (lvSNNumber_[index] == snNumber)
        {
            std::lock_guard lock(mtxCamera_);
            delete lvCameras_[index];
            lvCameras_[index] = nullptr;
            LogInfo("sn number {}, remove success.", snNumber);
        }
    }
}

bool BaumerManager::setCamera(const Json::Value &param, std::string &des)
{
    bool ret = true;
    uint8_t index = param["qml_window"].asInt();
    Camera *pCamera = nullptr;
    if (index < lvCameras_.size())
    {
        std::lock_guard lock(mtxCamera_);
        pCamera = lvCameras_[index];
    }
    if (pCamera == nullptr)
    {
        des = "camera init error.";
        return false;
    }
    pCamera->stopStream();
    for (auto it = param.begin(); it != param.end(); ++it)
    {
        const std::string &key = it.key().asString();
        const std::string &strValue = it->asString();
        uint16_t value = 0;
        if (key == "expose")
        {
            double temp = Utils::anyFromString<double>(strValue);
            lvParams_[index][key] = temp;
            value = temp * 1000.0;
        }
        else
        {
            value = Utils::anyFromString<uint64_t>(strValue);
            lvParams_[index][key] = value;
        }
        if (!pCamera->setParams(key, value))
        {
            des = key + "set error.";
            ret = false;
            break;
        }
    }
    pCamera->startStream();
    return ret;
}

Json::Value BaumerManager::getCamera(uint8_t number)
{
    Json::Value ret;
    if (number < lvCameras_.size())
    {
        std::lock_guard lock(mtxCamera_);
        ret = lvCameras_[number]->getROParams();
    }
    auto &temp = lvParams_[number];
    for (const auto &key : temp.getMemberNames())
    {
        ret[key] = temp[key];
    }
    return ret;
}
