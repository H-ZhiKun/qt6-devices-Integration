#pragma once
// 警告： 根据baumer官方提供的方法，我试过多次，已经是比较完美的封装了，最好别改，异常太多。！！！！
// 参数设置接口，不具有抽象条件。1 调用规范看起来相似，实则细节不同 2 统一使用异常，一个接口异常还会影响其他的参数设置
// 保持通信状态的线程，是实验多次后，较优的处理方式，为确保实时判断当前摄像头的图像采集是否正常
// 相机参数设置中，与图像相关的一些参数，如果在采集过程中是处于锁定状态的，所以要修改参数需要停止采集，设置完成后继续采集

#include "bgapi2_genicam/bgapi2_genicam.hpp"
#include <QImage>
#include <functional>
#include <mutex>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <queue>
#include <string>
#include <thread>
#include <vector>

// 表示一个BGAPI相机对象和其他相机特定信息的类
enum class CameraParams
{
    TriggerMode = 0,
    ExposureTime,
    MaxExposureTime,
    AutoExposureTime,
    Gain,
    AutoGain,
    MaxWidth,
    MaxHeight,
    RoiWidth,
    RoiHeight,
    OffsetX,
    OffsetY,
    FPS
};

class Camera
{
  public:
    explicit Camera(BGAPI2::Device *bgapi_device);
    virtual ~Camera();

    bool getInitialized();
    std::string getSNNumber(); // 获取 SN 序列号

    void storeImg(unsigned char *bayerRG8Data, const std::string &pixFormat, uint64_t width, uint64_t height,
                  uint64_t frameId);
    cv::Mat getImage();

    bool setParams(CameraParams key, uint64_t value);
    uint64_t getParams(CameraParams key);
    bool startStream(); // 打开图像采集流
    bool stopStream();  // 打开图像采集流
  private:
    void initialize();         // 初始化入口
    bool openDevice();         // 打开相机
    bool addBuffersToStream(); // 为相机添加缓存

    void keepCaptureRunning(); // 保持采集

    void deinitialize();           // 资源回收入口
    void clearBuffersFromStream(); // 清理相机缓存buffer
    void stopDevice();             // 关闭相机

    cv::Mat bayerRG8ToMat(unsigned char *buffer, uint64_t width, uint64_t height);
    cv::Mat mono10ToMat(unsigned char *buffer, uint64_t width, uint64_t height);
    cv::Mat mono8ToMat(unsigned char *buffer, uint64_t width, uint64_t height);

    int64_t switchParams(const std::string &key, uint64_t value = 0, bool bReadOnly = true);
    uint64_t readMaxValue(const std::string &key);
    void initParams();

  private:
    bool bActive_ = false;                 // 相机是否采集
    bool bInited_ = false;                 // 相机初始化完成
    bool bOpen_ = false;                   // 相机打开状态
    std::mutex mtxBuffer_;                 // 缓冲区 互斥量
    uint64_t frameId_ = 0;                 // 当前帧id
    uint64_t frameIdPrevious_ = 0;         // 上一帧id
    cv::Mat curImage_;                     // 当前最新采集图片
    std::queue<QImage> lvBuffer_;          // 图像对象 缓冲区
    BGAPI2::Device *cameraPtr_ = nullptr;  // BGAPI相机对象
    BGAPI2::DataStream *stream_ = nullptr; // 流对象
    bool chunk_was_active_ = false;        // 初始化时指示块模式是否处于活动状态的标志
    std::thread thObserver_;
    std::atomic_bool bHolder_ = true;
    std::list<BGAPI2::Buffer *> streamBuffers_;
    std::unordered_map<CameraParams, uint64_t> mapCameraParams_;
    std::unordered_map<CameraParams, std::string> mapCameraString_;
};
