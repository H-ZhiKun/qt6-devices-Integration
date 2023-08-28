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
#include <yaml-cpp/yaml.h>
class BaumerManager
{
  public:
    explicit BaumerManager();
    virtual ~BaumerManager();
    void start(const YAML::Node &launchConfig);
    void searchCamera(); // 搜索相机
    bool setCamera(const Json::Value &param, std::string &des);
    Json::Value getCamera(uint8_t number);           // 根据配置窗口号获取对应相机的参数列表
    std::list<cv::Mat> getImageBySN(uint8_t number); // 根据sn号获取对应相机的图片
    void removeCamera(const std::string &snNumber);

  protected:
    void initializeBGAPI();
    void deinitializeBGAPI();
    void stop();
    bool addCamera(const std::string &snNumber, BGAPI2::Device *dev);

  private:
    // 以下是sdk中的指针树结构，我尝试过很多次保存指针来管理对象的方式，均以异常结果。
    // 故只能沿用其sdk中对于对象指针的管理和索引方式。无法重构其数据结构。
    std::mutex mtxCamera_;
    std::vector<Json::Value> lvParams_;
    std::vector<std::string> lvSNNumber_;
    std::vector<Camera *> lvCameras_;
    BGAPI2::System *pSystem_ = nullptr;
    BGAPI2::Interface *pInterface_ = nullptr;
    std::string MACAddress;
};
