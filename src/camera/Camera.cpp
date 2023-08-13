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
    mapCameraString_[CameraParams::TriggerMode] = SFNC_TRIGGERMODE;
    mapCameraString_[CameraParams::ExposureTime] = SFNC_EXPOSURETIME;
    mapCameraString_[CameraParams::Gain] = SFNC_GAIN;
    mapCameraString_[CameraParams::RoiWidth] = SFNC_WIDTH;
    mapCameraString_[CameraParams::RoiHeight] = SFNC_HEIGHT;
    mapCameraString_[CameraParams::OffsetX] = SFNC_OFFSETX;
    mapCameraString_[CameraParams::OffsetY] = SFNC_OFFSETY;

    // keepCaptureRunning();
    initialize();
}

Camera::~Camera()
{
    bHolder_ = false;
    bActive_ = false;
    deinitialize();
    if (thObserver_.joinable())
        thObserver_.join();
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
        cameraPtr_->OpenExclusive();
        ret = true;
    }
    catch (BGAPI2::Exceptions::IException &ex)
    {
        qDebug() << "Error Type: " << ex.GetType();
        qDebug() << "Error function: " << ex.GetFunctionName();
        qDebug() << "Error description: " << ex.GetErrorDescription();
    }

    try
    {
        if (cameraPtr_->GetRemoteNodeList()->GetNodePresent(SFNC_CHUNKMODEACTIVE))
        {
            chunk_was_active_ = cameraPtr_->GetRemoteNode(SFNC_CHUNKMODEACTIVE)->GetBool();
        }
        if (cameraPtr_->GetRemoteNodeList()->GetNodePresent(SFNC_CHUNKSELECTOR))
        {
            for (BGAPI2::NodeMap::iterator iter =
                     cameraPtr_->GetRemoteNode(SFNC_CHUNKSELECTOR)->GetEnumNodeList()->begin();
                 iter != cameraPtr_->GetRemoteNode(SFNC_CHUNKSELECTOR)->GetEnumNodeList()->end(); iter++)
            {
                BGAPI2::Node *chunknode_name = (*iter).second.second;
                std::string chunk_name = chunknode_name->GetValue().get();
                if (chunknode_name->GetImplemented() && chunknode_name->GetAvailable())
                {
                    cameraPtr_->GetRemoteNode(SFNC_CHUNKSELECTOR)->SetValue(chunk_name.c_str());
                    cameraPtr_->GetRemoteNode(SFNC_CHUNKENABLE)->SetBool(true);
                }
            }
            if (!chunk_was_active_)
            {
                cameraPtr_->GetRemoteNode(SFNC_CHUNKMODEACTIVE)->SetBool(true);
            }
        }
    }
    catch (BGAPI2::Exceptions::IException &ex)
    {
        ret = false;
        qDebug() << "Error Type: " << ex.GetType();
        qDebug() << "Error function: " << ex.GetFunctionName();
        qDebug() << "Error description: " << ex.GetErrorDescription();
    }

    try
    {
        BGAPI2::DataStreamList *datastream_list = cameraPtr_->GetDataStreams();
        datastream_list->Refresh();
        if (datastream_list->size() > 0)
        {
            stream_ = datastream_list->begin()->second;
            stream_->Open();
            stream_->Close();
        }
    }
    catch (BGAPI2::Exceptions::IException &ex)
    {
        ret = false;
        qDebug() << "Error Type: " << ex.GetType();
        qDebug() << "Error function: " << ex.GetFunctionName();
        qDebug() << "Error description: " << ex.GetErrorDescription();
    }

    return ret;
}

bool Camera::addBuffersToStream()
{
    bool ret = false;
    try
    {
        cameraPtr_->GetDataStreams()->Refresh();
        stream_->Open();
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
        qDebug() << "Error Type: " << ex.GetType();
        qDebug() << "Error function: " << ex.GetFunctionName();
        qDebug() << "Error description: " << ex.GetErrorDescription();
    }
    return ret;
}

bool Camera::startStream()
{
    bool ret = false;
    try
    {
        stream_->GetBufferList()->DiscardAllBuffers();
        stream_->GetBufferList()->FlushAllToInputQueue();
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
            bInited_ = true;
            ret = true;
        }
    }
    catch (BGAPI2::Exceptions::IException &ex)
    {
        bInited_ = false;
        qDebug() << "Error Type: " << ex.GetType();
        qDebug() << "Error function: " << ex.GetFunctionName();
        qDebug() << "Error description: " << ex.GetErrorDescription();
    }
    return ret;
}

void Camera::keepCaptureRunning()
{
    thObserver_ = std::thread([this] {
        uint64_t frame = 0;
        while (bHolder_)
        {
            std::this_thread::sleep_for(std::chrono::seconds(5));
        }
    });
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
        if (cameraPtr_->GetRemoteNode(SFNC_ACQUISITION_STOP)->IsWriteable())
        {
            cameraPtr_->GetRemoteNode(SFNC_ACQUISITION_STOP)->Execute();
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
        stream_->StopAcquisition();
        ret = true;
    }
    catch (BGAPI2::Exceptions::IException &ex)
    {
        qDebug() << "Error Type: " << ex.GetType();
        qDebug() << "Error function: " << ex.GetFunctionName();
        qDebug() << "Error description: " << ex.GetErrorDescription();
    }
    return ret;
}

void Camera::clearBuffersFromStream()
{

    try
    {
        if (stream_ != nullptr)
        {
            stream_->RegisterNewBufferEvent(BGAPI2::Events::EVENTMODE_UNREGISTERED);
            stream_->UnregisterNewBufferEvent();
            BGAPI2::BufferList *buffer_list = stream_->GetBufferList();
            buffer_list->DiscardAllBuffers();
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
        qDebug() << "Error Type: " << ex.GetType();
        qDebug() << "Error function: " << ex.GetFunctionName();
        qDebug() << "Error description: " << ex.GetErrorDescription();
    }
}

void Camera::stopDevice()
{
    if (!chunk_was_active_ && bOpen_)
    {
        try
        {
            if (cameraPtr_->GetRemoteNodeList()->GetNodePresent(SFNC_CHUNKSELECTOR))
            {
                for (BGAPI2::NodeMap::iterator iter =
                         cameraPtr_->GetRemoteNode(SFNC_CHUNKSELECTOR)->GetEnumNodeList()->begin();
                     iter != cameraPtr_->GetRemoteNode(SFNC_CHUNKSELECTOR)->GetEnumNodeList()->end(); iter++)
                {
                    BGAPI2::Node *chunk_name = (*iter).second.second;
                    cameraPtr_->GetRemoteNode(SFNC_CHUNKSELECTOR)->SetValue(chunk_name->GetValue());
                    cameraPtr_->GetRemoteNode(SFNC_CHUNKENABLE)->SetBool(false);
                }
                cameraPtr_->GetRemoteNode(SFNC_CHUNKMODEACTIVE)->SetBool(false);
            }
        }
        catch (BGAPI2::Exceptions::IException &ex)
        {
            qDebug() << "Error Type: " << ex.GetType();
            qDebug() << "Error function: " << ex.GetFunctionName();
            qDebug() << "Error description: " << ex.GetErrorDescription();
        }
    }
    try
    {
        if (cameraPtr_->IsOpen())
        {
            cameraPtr_->Close();
        }
    }
    catch (BGAPI2::Exceptions::IException &ex)
    {
        qDebug() << "Error Type: " << ex.GetType();
        qDebug() << "Error function: " << ex.GetFunctionName();
        qDebug() << "Error description: " << ex.GetErrorDescription();
    }
}

bool Camera::getInitialized()
{
    return bInited_;
}

void Camera::storeImg(unsigned char *buffer, const std::string &pixFormat, uint64_t width, uint64_t height,
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

int64_t Camera::switchParams(const std::string &key, uint64_t value, bool bReadOnly)
{
    int64_t ret = -1;
    try
    {
        qDebug() << "switchParams: ";
        qDebug() << "key: " << key;
        qDebug() << "value: " << value;
        BGAPI2::Node *node = cameraPtr_->GetRemoteNode(key.c_str());
        if (node == nullptr)
        {
            return ret;
        }
        if (bReadOnly)
        {
            if (node->IsReadable() == false)
            {
                return ret;
            }
        }
        else
        {
            if (node->IsWriteable() == false)
            {
                return ret;
            }
        }

        BGAPI2::String valueType = node->GetInterface();
        if (valueType == "IEnumeration")
        {
            if (!bReadOnly)
            {
                node->SetString(value ? "On" : "Off");
            }
            ret = (node->GetString() == "On") ? 1 : 0;
        }
        if (valueType == "IInteger")
        {
            if (!bReadOnly)
            {
                uint64_t scale = 1;
                if (node->HasInc())
                {
                    scale = node->GetIntInc();
                }
                node->SetInt(value / scale * scale);
            }
            ret = static_cast<uint64_t>(node->GetInt());
        }
        if (valueType == "IFloat")
        {
            if (!bReadOnly)
            {
                node->SetDouble(value);
            }
            ret = static_cast<uint64_t>(node->GetDouble());
        }
    }
    catch (BGAPI2::Exceptions::IException &ex)
    {
        qDebug() << "Error switchParams: " << key;
        qDebug() << "Error Type: " << ex.GetType();
        qDebug() << "Error function: " << ex.GetFunctionName();
        qDebug() << "Error description: " << ex.GetErrorDescription();
    }
    return ret;
}

uint64_t Camera::readMaxValue(const std::string &key)
{
    uint64_t ret = 0;
    try
    {
        BGAPI2::Node *node = cameraPtr_->GetRemoteNode(key.c_str());
        if (node == nullptr)
        {
            return ret;
        }

        if (node->IsReadable() == false)
        {
            return ret;
        }

        BGAPI2::String valueType = node->GetInterface();
        if (valueType == "IInteger")
        {
            ret = static_cast<uint64_t>(node->GetIntMax());
        }
        if (valueType == "IFloat")
        {
            ret = static_cast<uint64_t>(node->GetDoubleMax());
        }
    }
    catch (BGAPI2::Exceptions::IException &ex)
    {
        qDebug() << "Error readMaxValue: " << key;
        qDebug() << "Error Type: " << ex.GetType();
        qDebug() << "Error function: " << ex.GetFunctionName();
        qDebug() << "Error description: " << ex.GetErrorDescription();
    }
    return ret;
}

bool Camera::setParams(CameraParams key, uint64_t value)
{
    bool ret = false;
    auto iter = mapCameraString_.find(key);
    if (iter != mapCameraString_.end())
    {
        auto temp = mapCameraParams_[key];
        if (temp != value)
        {
            temp = switchParams(iter->second, value, false);
            if (temp >= 0)
            {
                ret = true;
                mapCameraParams_[key] = temp;
            }
        }
    }
    return ret;
}

uint64_t Camera::getParams(CameraParams key)
{
    return mapCameraParams_[key];
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
    mapCameraParams_[CameraParams::TriggerMode] = switchParams(SFNC_TRIGGERMODE, 0, false);
    mapCameraParams_[CameraParams::ExposureTime] = switchParams(SFNC_EXPOSURETIME);
    mapCameraParams_[CameraParams::Gain] = switchParams(SFNC_GAIN);
    mapCameraParams_[CameraParams::RoiWidth] = switchParams(SFNC_WIDTH);
    mapCameraParams_[CameraParams::RoiHeight] = switchParams(SFNC_HEIGHT);
    mapCameraParams_[CameraParams::OffsetX] = switchParams(SFNC_OFFSETX);
    mapCameraParams_[CameraParams::OffsetY] = switchParams(SFNC_OFFSETY);
    mapCameraParams_[CameraParams::FPS] = switchParams(SFNC_ACQUISITION_FRAMERATE);
    mapCameraParams_[CameraParams::MaxExposureTime] = readMaxValue(SFNC_EXPOSURETIME);
    mapCameraParams_[CameraParams::AutoExposureTime] = switchParams(SFNC_EXPOSUREAUTO, 0, false);
    mapCameraParams_[CameraParams::MaxWidth] = switchParams(SFNC_WIDTHMAX);
    mapCameraParams_[CameraParams::MaxHeight] = switchParams(SFNC_HEIGHTMAX);
    mapCameraParams_[CameraParams::AutoGain] = switchParams(SFNC_GAINAUTO, 0, false);
}
