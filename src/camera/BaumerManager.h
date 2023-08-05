#pragma once

// 警告： 根据baumer官方提供的方法，我试过多次，已经是比较完美的封装了，最好别改，异常太多。！！！！
#include "Camera.h"
#include "bgapi2_genicam/bgapi2_genicam.hpp"
#include "json/json.h"
#include <QImage>
#include <atomic>
#include <list>
#include <mutex>
#include <shared_mutex>
#include <string>
#include <thread>
#include <unordered_map>
class BaumerManager
{
  public:
    explicit BaumerManager(Json::Value jsParam);
    virtual ~BaumerManager();
    void start();
    bool setCameraParam(const Json::Value &param);
    Json::Value getCameraParam(const std::string &SN); // 根据sn号获取对应相机的参数列表
    cv::Mat getImageBySN(const std::string &SN);       // 根据sn号获取对应相机的图片
    std::list<std::string> getCameraList();            // 返回相机列表sn号
    void removeCamera(const std::string &snNumber);

  protected:
    void initializeBGAPI();
    void deinitializeBGAPI();
    void stop();
    void addCamera(const std::string &snNumber, BGAPI2::Device *dev);
    void addSNNumber(const std::string &snNumber);
    void updateLocalParams(const std::string &SN, Camera *pCamera); // 更新本地对应SN相机参数

  private:
    // 以下是sdk中的指针树结构，我尝试过很多次保存指针来管理对象的方式，均以异常结果。
    // 故只能沿用其sdk中对于对象指针的管理和索引方式。无法重构其数据结构。
    std::shared_mutex mtxSN_;
    std::list<std::string> lvSNNumber;
    std::shared_mutex mtxCamera_;
    std::unordered_map<std::string, Camera *> mapCameras_;
    std::unordered_map<std::string, Json::Value> mapParams_;
    std::atomic_bool managerRun_{true};
    std::thread thSearchDev_;
    std::unordered_map<std::string, CameraParams> mapStringToParams_;
    std::unordered_map<CameraParams, std::string> mapParamsToString_;
};
