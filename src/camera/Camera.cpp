/* Copyright 2019-2020 Baumer Optronic */
#include "Camera.h"
#include "BufferInformation.h"
#include "Logger.h"
#include "Utils.h"
#include <QDebug>
#include <QThread>
#include <sstream>
#include <string>

void BGAPI2CALL BufferHandler(void *callBackOwner, BGAPI2::Buffer *pBufferFilled)
{
    try
    {
        Camera *pCamera = static_cast<Camera *>(callBackOwner);
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
        qDebug() << "Error Type: " << ex.GetType();
        qDebug() << "Error function: " << ex.GetFunctionName();
        qDebug() << "Error description: " << ex.GetErrorDescription();
    }
}

Camera::Camera(BGAPI2::Device *bgapi_device) : cameraPtr_(bgapi_device)
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

Camera::~Camera()
{
    bActive_ = false;
    deinitialize();
}

void Camera::initialize()
{
    if (openDevice())
    {
        bOpen_ = true;
        if (addBuffersToStream())
        {
            startStream();
        }
        initParams();
    }
}

bool Camera::openDevice()
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

bool Camera::addBuffersToStream()
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

bool Camera::startStream()
{
    bool ret = false;
    try
    {
        stream_->StartAcquisitionContinuous();
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
            ret = true;
        }
    }
    catch (BGAPI2::Exceptions::IException &ex)
    {
        bActive_ = false;
        LogError("Error Type: {}", ex.GetType().get());
        LogError("Error function: {}", ex.GetFunctionName().get());
        LogError("Error description: {}", ex.GetErrorDescription().get());
    }
    return ret;
}

void Camera::deinitialize()
{
    stopStream();
    clearBuffersFromStream();
    stopDevice();
    // 由这里统一释放，避免异常导致的泄漏问题
    for (auto &it : streamBuffers_)
    {
        delete it;
    }
    streamBuffers_.clear();
}

bool Camera::stopStream()
{
    bool ret = false;
    bActive_ = false;
    if (stream_ == nullptr)
    {
        return ret;
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

    try
    {
        stream_->StopAcquisition();
        auto bufferList = stream_->GetBufferList();
        bufferList->DiscardAllBuffers();
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

void Camera::clearBuffersFromStream()
{

    try
    {
        if (stream_ != nullptr)
        {
            stream_->UnregisterNewBufferEvent();
            // stream_->RegisterNewBufferEvent(BGAPI2::Events::EVENTMODE_UNREGISTERED);
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

void Camera::stopDevice()
{
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

bool Camera::getInitialized()
{
    return bActive_;
}

void Camera::storeImg(unsigned char *buffer, const std::string &pixFormat, uint64_t width, uint64_t height,
                      uint64_t frameId)
{
    if (matBuffers_.getSize() > 10)
    {
        matBuffers_.clear();
    }
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
    matBuffers_.enqueue(std::move(mat));
}

std::list<cv::Mat> Camera::getImage()
{
    std::list<cv::Mat> list;
    while (!matBuffers_.empty())
    {
        cv::Mat mat;
        matBuffers_.dequeue(mat);
        if (!mat.empty())
        {
            list.emplace_back(std::move(mat));
        }
    }
    return list;
}

cv::Mat Camera::mono8ToMat(unsigned char *buffer, uint64_t width, uint64_t height)
{
    cv::Mat matImage(height, width, CV_8UC1, buffer);
    return matImage;
}

bool Camera::writeParam(const std::string &key, uint64_t value)
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
            uint16_t scale = 0;
            if (node->HasInc())
            {
                scale = node->GetIntInc();
            }
            value = value / scale * scale;
            node->SetInt(value);
        }
        if (valueType == "IFloat")
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

bool Camera::setParams(const std::string &key, uint64_t value)
{
    auto iter = mapCameraString_.find(key);
    if (iter != mapCameraString_.end())
    {
        return writeParam(iter->second, value);
    }
    return false;
}

const Json::Value &Camera::getROParams()
{
    return jsReadOnly_;
}

cv::Mat Camera::bayerRG8ToMat(unsigned char *buffer, uint64_t width, uint64_t height)
{
    cv::Mat imOriginal(width, height, CV_8UC1, buffer);
    cv::Mat imTransformBGR8(height, width, CV_8UC3);
    // Baumer: RGrgrg  >>  OpenCV: rgrgrg
    //         gbgbgb              gBGbgb
    cv::cvtColor(imOriginal, imTransformBGR8, cv::COLOR_BayerBG2BGR); // to BGR
    return imTransformBGR8;
}

cv::Mat Camera::mono10ToMat(unsigned char *buffer, uint64_t width, uint64_t height)
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

void Camera::initParams()
{
    writeParam(SFNC_EXPOSUREAUTO, 0);
    writeParam(SFNC_GAINAUTO, 0);
    jsReadOnly_["fps"] = cameraPtr_->GetRemoteNode(SFNC_ACQUISITION_FRAMERATE)->GetInt();
    jsReadOnly_["max_width"] = cameraPtr_->GetRemoteNode(SFNC_WIDTHMAX)->GetInt();
    jsReadOnly_["max_height"] = cameraPtr_->GetRemoteNode(SFNC_HEIGHTMAX)->GetInt();
    jsReadOnly_["width_increment"] = cameraPtr_->GetRemoteNode(SFNC_WIDTH)->GetIntInc();
    jsReadOnly_["height_increment"] = cameraPtr_->GetRemoteNode(SFNC_HEIGHT)->GetIntInc();
    jsReadOnly_["offsetx_increment"] = cameraPtr_->GetRemoteNode(SFNC_OFFSETX)->GetIntInc();
    jsReadOnly_["offsety_increment"] = cameraPtr_->GetRemoteNode(SFNC_OFFSETY)->GetIntInc();
    qDebug() << jsReadOnly_.toStyledString();
}
