#pragma once

// 警告： 根据baumer官方提供的方法，我试过多次，已经是比较完美的封装了，最好别改，异常太多。！！！！
#include "BaumerCamera.h"
#include "bgapi2_genicam/bgapi2_genicam.hpp"
#include <QImage>
#include <QObject>
#include <atomic>
#include <cstdint>
#include <mutex>
#include <string>
#include <thread>
#include <unordered_map>
#include <yaml-cpp/yaml.h>

struct ImagePackage
{
    uint32_t number = 0;
    cv::Mat image;
};

class CameraManager : public QObject
{
    Q_OBJECT
  public:
    explicit CameraManager();
    virtual ~CameraManager();
    virtual void start(const YAML::Node &launchConfig);
    void cleanCameraCount();
    void resetCameraCount(const std::string &key, const uint32_t count);
    void removeCamera(const std::string &snNumber);
    uint32_t getCameraCount(const std::string);

    std::vector<std::string> cameraState();
  signals:
    void caputureImage(const std::string &cameraType, uint32_t imgNumber, const cv::Mat &image);

  protected:
    void getCameraImage(); // 获取对应相机的图片并发送采集信号给外部使用
    void initializeBGAPI();
    void deinitializeBGAPI();
    void stop();
    void searchCamera(); // 搜索相机
    bool addCamera(const std::string &snNumber, BGAPI2::Device *dev);
    bool forceIP(const std::string &snNumber, BGAPI2::NodeMap *nodeMap);

  private:
    // 以下是sdk中的指针树结构，我尝试过很多次保存指针来管理对象的方式，均以异常结果。
    // 故只能沿用其sdk中对于对象指针的管理和索引方式。无法重构其数据结构。
    std::thread thSearch_;
    std::thread thCaputure_;
    std::atomic_bool bHold_{true};
    std::mutex mtxCamera_;
    std::string interfaceIp_;
    std::string interfaceMask_;
    std::string interfaceMAC_;
    uint32_t u32InterfaceIp_ = 0;
    uint32_t u32InterfaceMask_ = 0;
    YAML::Node nodeParams_;
    std::unordered_map<std::string, std::unique_ptr<BaumerCamera>> mapCameras_;
    std::mutex mtxCount_;
    std::unordered_map<std::string, uint32_t> mapCameraCounts_;
    BGAPI2::System *pSystem_ = nullptr;
};