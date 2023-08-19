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
            qDebug() << "camera remove singal: " << SNNumber;
            pManager->removeCamera(SNNumber);
        }
        else if (EventTypeStr == "added")
        {
            //   改为主动添加
            // qDebug() << "camera add  singal" << SNNumber;
            // pManager->searchCamera();
        }
    }
}

BaumerManager::BaumerManager(Json::Value jsParam)
{
    mapStringToParams_["max_width"] = CameraParams::MaxWidth;
    mapStringToParams_["max_height"] = CameraParams::MaxHeight;
    mapStringToParams_["trigger_mode"] = CameraParams::TriggerMode;
    mapStringToParams_["expose"] = CameraParams::ExposureTime;
    mapStringToParams_["max_expose"] = CameraParams::MaxExposureTime;
    mapStringToParams_["fps"] = CameraParams::FPS;
    mapStringToParams_["gain"] = CameraParams::Gain;
    mapStringToParams_["width"] = CameraParams::RoiWidth;
    mapStringToParams_["height"] = CameraParams::RoiHeight;
    mapStringToParams_["offset_x"] = CameraParams::OffsetX;
    mapStringToParams_["offset_y"] = CameraParams::OffsetY;
    for (auto &[key, value] : mapStringToParams_)
    {
        mapParamsToString_[value] = key;
    }
    for (auto &item : jsParam)
    {
        std::string sn = item["sn_num"].asString();
        mapParams_[sn] = item;
    }
}

BaumerManager::~BaumerManager()
{
    stop();
}

void BaumerManager::start()
{
    initializeBGAPI();
    thSearchDev_ = std::thread([this] {
        while (managerRun_)
        {
            searchCamera();
        }
    });
}

void BaumerManager::searchCamera()
{
    try
    {
        BGAPI2::SystemList *system_list = BGAPI2::SystemList::GetInstance();
        system_list->Refresh();
        for (BGAPI2::SystemList::iterator sys_iter = system_list->begin(); sys_iter != system_list->end(); sys_iter++)
        {
            BGAPI2::System *system_pointer = sys_iter->second;
            BGAPI2::String tl_type = system_pointer->GetTLType();
            if (system_pointer->IsOpen())
            {
                BGAPI2::InterfaceList *interface_list = system_pointer->GetInterfaces();
                interface_list->Refresh(500);
                for (BGAPI2::InterfaceList::iterator ifc_iter = interface_list->begin();
                     ifc_iter != interface_list->end(); ifc_iter++)
                {
                    if (ifc_iter->second->IsOpen())
                    {
                        BGAPI2::DeviceList *device_list = ifc_iter->second->GetDevices();
                        device_list->Refresh(500);

                        for (BGAPI2::DeviceList::iterator device_iter = device_list->begin();
                             device_iter != device_list->end(); device_iter++)
                        {
                            std::string SN = device_iter->second->GetSerialNumber().get();
                            auto dev = device_iter->second;
                            std::string status = dev->GetAccessStatus().get();
                            if (!SN.empty() && status == "RW")
                            {
                                addCamera(SN, dev);
                            }
                        }
                    }
                }
            }
        }
    }
    catch (BGAPI2::Exceptions::IException &ex)
    {
        qDebug() << "Error Type: " << ex.GetType();
        qDebug() << "Error function: " << ex.GetFunctionName();
        qDebug() << "Error description: " << ex.GetErrorDescription();
    }
    std::this_thread::sleep_for(std::chrono::seconds(5));
}

void BaumerManager::stop()
{
    managerRun_ = false;
    thSearchDev_.join();
    if (mapCameras_.size() > 0)
    {
        std::unique_lock lock(mtxCamera_);
        for (auto [SN, camera] : mapCameras_)
        {
            delete camera;
        }
        mapCameras_.clear();
    }
    deinitializeBGAPI();
}

std::list<cv::Mat> BaumerManager::getImageBySN(const std::string &SN)
{
    if (!SN.empty())
    {
        std::shared_lock lock(mtxCamera_);
        auto iter = mapCameras_.find(SN);
        if (iter != mapCameras_.end() && iter->second != nullptr)
        {
            return iter->second->getImage();
        }
    }
    return {};
};

std::list<std::string> BaumerManager::getCameraList()
{
    std::shared_lock lock(mtxSN_);
    return lvSNNumber;
}

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

            if (!system_pointer->IsOpen())
            {
                system_pointer->Open();
            }

            BGAPI2::InterfaceList *interface_list = system_pointer->GetInterfaces();
            interface_list->Refresh(500);
            for (BGAPI2::InterfaceList::iterator ifc_iter = interface_list->begin(); ifc_iter != interface_list->end();
                 ifc_iter++)
            {
                if (!ifc_iter->second->IsOpen())
                {
                    ifc_iter->second->Open();
                    // 为相机对应数据流注册掉线触发事件
                    ifc_iter->second->RegisterPnPEvent(BGAPI2::Events::EVENTMODE_EVENT_HANDLER);
                    ifc_iter->second->RegisterPnPEventHandler(this, (Events::PnPEventHandler)&PnPEventHandler);
                }
                BGAPI2::DeviceList *device_list = ifc_iter->second->GetDevices();
                device_list->Refresh(500);
                if (device_list->size() == 0)
                {
                    ifc_iter->second->Close();
                }
            }
        }
    }
    catch (BGAPI2::Exceptions::IException &ex)
    {
        qDebug() << "Error Type: " << ex.GetType();
        qDebug() << "Error function: " << ex.GetFunctionName();
        qDebug() << "Error description: " << ex.GetErrorDescription();
    }
    std::this_thread::sleep_for(std::chrono::seconds(5));
}

void BaumerManager::deinitializeBGAPI()
{
    try
    {
        BGAPI2::SystemList *system_list = BGAPI2::SystemList::GetInstance();
        system_list->Refresh();
        for (BGAPI2::SystemList::iterator sys_iter = system_list->begin(); sys_iter != system_list->end(); sys_iter++)
        {
            BGAPI2::System *system_pointer = sys_iter->second;
            BGAPI2::InterfaceList *interface_list = system_pointer->GetInterfaces();
            interface_list->Refresh(100);
            for (BGAPI2::InterfaceList::iterator ifc_iter = interface_list->begin(); ifc_iter != interface_list->end();
                 ifc_iter++)
            {
                if (ifc_iter->second->IsOpen())
                {
                    ifc_iter->second->RegisterPnPEvent(BGAPI2::Events::EVENTMODE_UNREGISTERED);
                    ifc_iter->second->UnregisterPnPEvent();
                    ifc_iter->second->Close();
                }
            }
        }
        system_list->Refresh();
        for (BGAPI2::SystemList::iterator sys_iter = system_list->begin(); sys_iter != system_list->end(); sys_iter++)
        {
            if (sys_iter->second->IsOpen())
                sys_iter->second->Close();
        }
    }
    catch (BGAPI2::Exceptions::IException &ex)
    {
        qDebug() << "Error Type: " << ex.GetType();
        qDebug() << "Error function: " << ex.GetFunctionName();
        qDebug() << "Error description: " << ex.GetErrorDescription();
    }
    try
    {
        BGAPI2::SystemList::ReleaseInstance();
    }
    catch (BGAPI2::Exceptions::IException &ex)
    {
        qDebug() << "Error Type: " << ex.GetType();
        qDebug() << "Error function: " << ex.GetFunctionName();
        qDebug() << "Error description: " << ex.GetErrorDescription();
    }
}

void BaumerManager::addCamera(const std::string &snNumber, BGAPI2::Device *dev)
{
    if (managerRun_ == false)
        return;
    auto iter = mapCameras_.find(snNumber);
    if (iter == mapCameras_.end())
    {
        Camera *camera_obj = new Camera(dev);
        if (camera_obj->getInitialized())
        {
            std::unique_lock lock(mtxCamera_);
            mapCameras_[snNumber] = camera_obj;
            qDebug() << "camera added " << snNumber;
        }
        else
        {
            if (camera_obj)
            {
                delete camera_obj;
                camera_obj = nullptr;
            }
        }
        if (camera_obj != nullptr)
        {
            addSNNumber(snNumber);
            auto iter = mapParams_.find(snNumber);
            if (iter != mapParams_.end())
            {
                setCameraParam(iter->second);
            }
            else
            {
                updateLocalParams(snNumber, camera_obj);
            }
        }
    }
    else
    {
        if (!iter->second->getInitialized())
        {

            delete iter->second;
            std::unique_lock lock(mtxCamera_);
            mapCameras_.erase(iter);
            qDebug() << "camera removed: " << snNumber;
        }
    }
}

void BaumerManager::removeCamera(const std::string &snNumber)
{
    if (managerRun_ == false)
        return;
    std::unique_lock lock(mtxCamera_);
    auto iter = mapCameras_.find(snNumber);
    if (iter != mapCameras_.end())
    {
        iter->second->stopStream();
        iter->second->startStream();
        // delete iter->second;
        // mapCameras_.erase(iter);
    }
}

void BaumerManager::addSNNumber(const std::string &snNumber)
{
    if (std::find(lvSNNumber.begin(), lvSNNumber.end(), snNumber) == lvSNNumber.end())
    {
        std::unique_lock lock(mtxSN_);
        lvSNNumber.push_back(snNumber);
    }
}

void BaumerManager::updateLocalParams(const std::string &SN, Camera *pCamera)
{
    Json::Value dist;
    dist["sn_num"] = SN;
    for (auto iter = mapParamsToString_.begin(); iter != mapParamsToString_.end(); iter++)
    {
        uint64_t value = pCamera->getParams(iter->first);
        if (CameraParams::ExposureTime == iter->first || CameraParams::MaxExposureTime == iter->first)
        {
            dist[iter->second] = static_cast<double>(value) / 1000.0;
        }
        else
        {
            dist[iter->second] = value;
        }
    }
    mapParams_[SN] = std::move(dist);
}

bool BaumerManager::setCameraParam(const Json::Value &param)
{
    bool ret = false;
    if (param.isNull())
    {
        return ret;
    }
    std::string snNum = param["sn_num"].asString();
    Camera *pCamera = nullptr;
    if (!snNum.empty())
    {
        std::shared_lock lock(mtxCamera_);
        pCamera = mapCameras_[snNum];
    }
    if (pCamera == nullptr)
    {
        return ret;
    }
    pCamera->stopStream();
    for (auto it = param.begin(); it != param.end(); ++it)
    {
        const std::string &key = it.key().asString();
        const std::string &strValue = it->asString();
        auto finded = mapStringToParams_.find(key);
        if (finded != mapStringToParams_.end())
        {
            uint64_t value = 0;
            if (finded->second == CameraParams::ExposureTime || finded->second == CameraParams::MaxExposureTime)
            {
                value = Utils::anyFromString<double>(strValue) * 1000.0;
            }
            else
            {
                value = Utils::anyFromString<uint64_t>(strValue);
            }
            ret = pCamera->setParams(finded->second, value);
        }
    }
    pCamera->startStream();
    ret = true;
    updateLocalParams(snNum, pCamera);
    return ret;
}

Json::Value BaumerManager::getCameraParam(const std::string &SN)
{
    Json::Value jsVal;
    auto iter = mapParams_.find(SN);
    if (iter != mapParams_.end())
    {
        jsVal = iter->second;
    }
    return jsVal;
}
