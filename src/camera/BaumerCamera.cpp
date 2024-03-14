/* Copyright 2019-2020 Baumer Optronic */
#include "BaumerCamera.h"
#include "Logger.h"
#include <QDebug>
#include <QThread>
#include <string>

void BGAPI2CALL BufferHandler(void *callBackOwner, BGAPI2::Buffer *pBufferFilled)
{
    try
    {
        BaumerCamera *pCamera = static_cast<BaumerCamera *>(callBackOwner);
        if (pBufferFilled == NULL)
        {
            return;
        }
        if (pBufferFilled->GetIsIncomplete())
        {
            pBufferFilled->QueueBuffer();
        }
        else
        {
            uint64_t width = pBufferFilled->GetWidth();
            uint64_t height = pBufferFilled->GetHeight();
            unsigned char *imageData =
                static_cast<unsigned char *>(pBufferFilled->GetMemPtr()) + pBufferFilled->GetImageOffset();
            pCamera->storeImg(imageData, pBufferFilled->GetPixelFormat().get(), width, height,
                              pBufferFilled->GetFrameID());
            pBufferFilled->QueueBuffer();
        }
    }
    catch (BGAPI2::Exceptions::IException &ex)
    {
        qDebug() << "Error Type: " << ex.GetType().get();
        qDebug() << "Error function: " << ex.GetFunctionName().get();
        qDebug() << "Error description: " << ex.GetErrorDescription().get();
    }
}

BaumerCamera::BaumerCamera(BGAPI2::Device *bgapi_device, const YAML::Node &config)
    : cameraPtr_(bgapi_device), paramConfig_(config)
{
    mapCameraString_["trigger_mode"] = SFNC_TRIGGERMODE;
    mapCameraString_["expose"] = SFNC_EXPOSURETIME;
    mapCameraString_["gain"] = SFNC_GAIN;
    mapCameraString_["width"] = SFNC_WIDTH;
    mapCameraString_["height"] = SFNC_HEIGHT;
    mapCameraString_["offset_x"] = SFNC_OFFSETX;
    mapCameraString_["offset_y"] = SFNC_OFFSETY;

    // keepCaptureRunning();
    initialize();
}

BaumerCamera::~BaumerCamera()
{
    bActive_ = false;
    deinitialize();
}

void BaumerCamera::initialize()
{
    if (openDevice())
    {
        bOpen_ = true;
        initParams();
        if (addBuffersToStream())
        {
            startCollect();
        }
    }
}

bool BaumerCamera::openDevice()
{
    bool ret = false;
    try
    {
        cameraPtr_->Open();
        ret = true;
    }
    catch (BGAPI2::Exceptions::ResourceInUseException &ex)
    {
        LogError("ResourceInUseException: ", ex.GetErrorDescription().get());
    }
    catch (BGAPI2::Exceptions::AccessDeniedException &ex)
    {
        LogError("AccessDeniedException: ", ex.GetErrorDescription().get());
    }
    try
    {
        BGAPI2::DataStreamList *datastream_list = cameraPtr_->GetDataStreams();
        datastream_list->Refresh();
        if (datastream_list->size() > 0)
        {
            stream_ = datastream_list->begin()->second;
            stream_->Open();
        }
    }
    catch (BGAPI2::Exceptions::IException &ex)
    {
        ret = false;
        LogError("Error Type: {}", ex.GetType().get());
        LogError("Error function: {}", ex.GetFunctionName().get());
        LogError("Error description: {}", ex.GetErrorDescription().get());
    }
    return ret;
}

bool BaumerCamera::addBuffersToStream()
{
    bool ret = false;
    try
    {
        cameraPtr_->GetDataStreams()->Refresh();
        BGAPI2::BufferList *bufferList = stream_->GetBufferList();
        for (int i = 0; i < 5; i++)
        {
            BGAPI2::Buffer *buffer = new BGAPI2::Buffer();
            bufferList->Add(buffer);
            buffer->QueueBuffer();
            streamBuffers_.push_back(buffer);
        }
        stream_->RegisterNewBufferEvent(BGAPI2::Events::EVENTMODE_EVENT_HANDLER);
        stream_->RegisterNewBufferEventHandler(this, (BGAPI2::Events::NewBufferEventHandler)&BufferHandler);
        stream_->StartAcquisitionContinuous();
        ret = true;
    }
    catch (BGAPI2::Exceptions::IException &ex)
    {
        LogError("Error Type: {}", ex.GetType().get());
        LogError("Error function: {}", ex.GetFunctionName().get());
        LogError("Error description: {}", ex.GetErrorDescription().get());
    }
    return ret;
}

void BaumerCamera::deinitialize()
{
    stopCollect();
    clearBuffersFromStream();
    stopDevice();
    // 由这里统一释放，避免异常导致的泄漏问题
    for (auto &it : streamBuffers_)
    {
        delete it;
    }
    streamBuffers_.clear();
}

void BaumerCamera::clearBuffersFromStream()
{
    try
    {
        if (stream_ != nullptr)
        {
            stream_->StopAcquisition();
            stream_->UnregisterNewBufferEvent();
            // stream_->RegisterNewBufferEvent(BGAPI2::Events::EVENTMODE_UNREGISTERED);
            auto bufferList = stream_->GetBufferList();
            bufferList->DiscardAllBuffers();
            BGAPI2::BufferList *buffer_list = stream_->GetBufferList();
            while (buffer_list->size() > 0)
            {
                BGAPI2::Buffer *buffer = buffer_list->begin()->second;
                buffer_list->RevokeBuffer(buffer);
            }
            stream_->Close();
            stream_ = nullptr;
        }
    }
    catch (BGAPI2::Exceptions::IException &ex)
    {
        LogError("Error Type: {}", ex.GetType().get());
        LogError("Error function: {}", ex.GetFunctionName().get());
        LogError("Error description: {}", ex.GetErrorDescription().get());
    }
}

void BaumerCamera::stopDevice()
{
    if (cameraPtr_ == nullptr)
    {
        return;
    }
    try
    {
        cameraPtr_->Close();
    }
    catch (BGAPI2::Exceptions::IException &ex)
    {
        LogError("Error Type: {}", ex.GetType().get());
        LogError("Error function: {}", ex.GetFunctionName().get());
        LogError("Error description: {}", ex.GetErrorDescription().get());
    }
}

bool BaumerCamera::getInitialized()
{
    return bActive_;
}

void BaumerCamera::storeImg(unsigned char *buffer, const std::string &pixFormat, uint64_t width, uint64_t height,
                            uint64_t frameId)
{
    cv::Mat mat;
    if (pixFormat == "BayerRG8")
    {
        mat = bayerRG8ToMat(buffer, width, height);
    }
    else if (pixFormat == "Mono10")
    {
        mat = mono10ToMat(buffer, width, height);
    }
    else if (pixFormat == "Mono8")
    {
        mat = mono8ToMat(buffer, width, height);
    }
    std::lock_guard lock(mtxCRT);
    matBuffers_.push(mat);
}

cv::Mat BaumerCamera::getCurrentMat()
{
    std::lock_guard lock(mtxCRT);
    cv::Mat mat;
    if (matBuffers_.empty())
    {
        return mat;
    }
    else
    {
        mat = matBuffers_.front();
        matBuffers_.pop();
        return mat;
    }
}

void BaumerCamera::startCollect()
{
    if (cameraPtr_ == nullptr)
    {
        return;
    }
    try
    {
        if (cameraPtr_->GetRemoteNodeList()->GetNodePresent(SFNC_TESTIMAGESELECTORVALUE_FRAMECOUNTER))
        {
            if (cameraPtr_->GetRemoteNode(SFNC_TESTIMAGESELECTORVALUE_FRAMECOUNTER)->IsWriteable())
            {
                cameraPtr_->GetRemoteNode(SFNC_TESTIMAGESELECTORVALUE_FRAMECOUNTER)->SetInt(0);
            }
        }
        if (cameraPtr_->GetRemoteNode(SFNC_ACQUISITION_START)->IsWriteable())
        {
            cameraPtr_->GetRemoteNode(SFNC_ACQUISITION_START)->Execute();
            bActive_ = true;
        }
    }
    catch (BGAPI2::Exceptions::IException &ex)
    {
        bActive_ = false;
        LogError("Error Type: {}", ex.GetType().get());
        LogError("Error function: {}", ex.GetFunctionName().get());
        LogError("Error description: {}", ex.GetErrorDescription().get());
    }
}

void BaumerCamera::stopCollect()
{
    bActive_ = false;
    if (cameraPtr_ == nullptr)
    {
        return;
    }
    try
    {
        if (cameraPtr_->GetRemoteNodeList()->GetNodePresent("AcquisitionAbort"))
        {
            cameraPtr_->GetRemoteNode("AcquisitionAbort")->Execute();
        }
        if (cameraPtr_->GetRemoteNode(SFNC_ACQUISITION_STOP)->IsWriteable())
        {
            cameraPtr_->GetRemoteNode(SFNC_ACQUISITION_STOP)->Execute();
        }
    }
    catch (BGAPI2::Exceptions::IException &ex)
    {
        LogError("Error Type: {}", ex.GetType().get());
        LogError("Error function: {}", ex.GetFunctionName().get());
        LogError("Error description: {}", ex.GetErrorDescription().get());
    }
}

cv::Mat BaumerCamera::mono8ToMat(unsigned char *buffer, uint64_t width, uint64_t height)
{
    cv::Mat matImage(height, width, CV_8UC1, buffer);
    return matImage;
}

bool BaumerCamera::writeParam(const std::string &key, uint64_t value)
{
    bool ret = false;
    try
    {
        BGAPI2::Node *node = cameraPtr_->GetRemoteNode(key.c_str());
        if (node == nullptr || node->IsWriteable() == false)
        {
            return ret;
        }

        BGAPI2::String valueType = node->GetInterface();
        if (valueType == "IEnumeration")
        {
            node->SetString(value ? "On" : "Off");
        }
        else if (valueType == "IInteger")
        {
            node->SetInt(value);
        }
        else if (valueType == "IFloat")
        {
            node->SetDouble(value);
        }
        ret = true;
    }
    catch (BGAPI2::Exceptions::IException &ex)
    {
        LogError("Error Type: {}", ex.GetType().get());
        LogError("Error function: {}", ex.GetFunctionName().get());
        LogError("Error description: {}", ex.GetErrorDescription().get());
    }
    return ret;
}

bool BaumerCamera::setParams(const std::string &key, uint64_t value)
{
    auto iter = mapCameraString_.find(key);
    if (iter != mapCameraString_.end())
    {
        return writeParam(iter->second, value);
    }
    else
    {
        return true;
    }
}

cv::Mat BaumerCamera::bayerRG8ToMat(unsigned char *buffer, uint64_t width, uint64_t height)
{
    cv::Mat imOriginal(width, height, CV_8UC1, buffer);
    cv::Mat imTransformBGR8(height, width, CV_8UC3);
    // Baumer: RGrgrg  >>  OpenCV: rgrgrg
    //         gbgbgb              gBGbgb
    cv::cvtColor(imOriginal, imTransformBGR8, cv::COLOR_BayerBG2BGR); // to BGR
    return imTransformBGR8;
}

cv::Mat BaumerCamera::mono10ToMat(unsigned char *buffer, uint64_t width, uint64_t height)
{
    // 创建一个新的cv::Mat对象来存储图像数据
    cv::Mat mat(height, width, CV_8UC1);

    // 解码Mono10图像数据
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            unsigned char lowerByte = buffer[2 * (y * width + x)];
            unsigned char upperByte = buffer[2 * (y * width + x) + 1];

            // 忽略最高位两位，只保留8位有效像素数据
            unsigned char pixelValue = (upperByte << 2) | (lowerByte >> 6);

            // 将像素值写入cv::Mat对象
            mat.at<unsigned char>(y, x) = pixelValue;
        }
    }
    return mat;
}

void BaumerCamera::initParams()
{
    try
    {

        // 设置数字IO控制
        cameraPtr_->GetRemoteNode("LineSelector")->SetString("Line0");
        cameraPtr_->GetRemoteNode("LineInverter")->SetBool(false); // high means high

        // 设置防抖信号参数
        // make sure only intended signals trigger the camera (ignore jitters on the line)
        cameraPtr_->GetRemoteNode("LineDebouncerHighTimeAbs")->SetDouble(1000); // in µsec
        cameraPtr_->GetRemoteNode("LineDebouncerLowTimeAbs")->SetDouble(1000);  // in µsec

        // 设置触发采集控制
        cameraPtr_->GetRemoteNode("TriggerMode")->SetString("On");
        cameraPtr_->GetRemoteNode("TriggerSource")->SetString("Line0");
        cameraPtr_->GetRemoteNode("TriggerActivation")->SetString("RisingEdge");
        cameraPtr_->GetRemoteNode("TriggerDelay")->SetDouble(0.0); // delay in µsec

        double expose = paramConfig_["expose"].as<double>() * 1000.0;
        int gain = paramConfig_["gain"].as<int>();
        int width = paramConfig_["width"].as<int>();
        int height = paramConfig_["height"].as<int>();
        int offset_x = paramConfig_["offset_x"].as<int>();
        int offset_y = paramConfig_["offset_y"].as<int>();
        cameraPtr_->GetRemoteNode(SFNC_EXPOSUREAUTO)->SetString("Off");
        cameraPtr_->GetRemoteNode(SFNC_GAINAUTO)->SetString("Off");
        cameraPtr_->GetRemoteNode(SFNC_EXPOSURETIME)->SetDouble(expose);
        cameraPtr_->GetRemoteNode(SFNC_GAIN)->SetInt(gain);
        cameraPtr_->GetRemoteNode(SFNC_WIDTH)->SetInt(width);
        cameraPtr_->GetRemoteNode(SFNC_HEIGHT)->SetInt(height);
        cameraPtr_->GetRemoteNode(SFNC_OFFSETX)->SetInt(offset_x);
        cameraPtr_->GetRemoteNode(SFNC_OFFSETY)->SetInt(offset_y);
        LogInfo("camera set params:\
                sn={}, \
                ip={}, \
                expose={}, \
                gain={}, \
                width={}, \
                height={}, \
                offset_x={}, \
                offset_y={}.",
                paramConfig_["sn_number"].as<std::string>(), paramConfig_["ip"].as<std::string>(), expose, gain, width,
                height, offset_x, offset_y);
    }
    catch (BGAPI2::Exceptions::IException &ex)
    {
        LogInfo("camera set params error:\
                sn={}, \
                ip={}.",
                paramConfig_["sn_number"].as<std::string>(), paramConfig_["ip"].as<std::string>());
        LogError("Error Type: {}", ex.GetType().get());
        LogError("Error function: {}", ex.GetFunctionName().get());
        LogError("Error description: {}", ex.GetErrorDescription().get());
    }
}
