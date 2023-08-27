#pragma once
#include "Logger.h"
#include "Utils.h"
#include <iostream>

class Product
{
  public:
    Product() = default;
    ~Product() = default;
    void setQrCodeRes(const std::string &str)
    {
        qrCodeRes = str;
    }
    void setLogisticsGT1(const std::string &str)
    {
        logistics1 = str;
    }
    void setLogisticsGT2(const std::string &str)
    {
        logistics2 = str;
    }
    void setBatchNum(const std::string &str)
    {
        batchNum = str;
    }
    void setFormulaName(const std::string &str)
    {
        formulaName = str;
    }
    void setLocateImageName(const std::string &str)
    {
        locateImageName = str;
    }
    void setLocateCheckImageName(const std::string &str)
    {
        locateCheckImageName = str;
    }
    void setCodeCheckImageName(const std::string &str)
    {
        codeCheckImageName = str;
    }
    void setLocateImage(const cv::Mat &image)
    {
        if (locateImage == nullptr)
        {
            locateImage = new cv::Mat(image.clone());
        }
        else
        {
            *locateImage = image;
        }
    }
    void setLocateCheckImage(const cv::Mat &image)
    {
        if (locateCheckImage == nullptr)
        {
            locateCheckImage = new cv::Mat(image.clone());
        }
        else
        {
            *locateCheckImage = image;
        }
    }
    void setCodeCheckImage(const cv::Mat &image)
    {
        if (codeCheckImage == nullptr)
        {
            codeCheckImage = new cv::Mat(image.clone());
        }
        else
        {
            *codeCheckImage = image;
        }
    }
    void setLocateRes(const float res)
    {
        locateRes = res;
    }
    void setLocateCheckRes(const bool res)
    {
        locateCheckRes = res;
    }
    void setLogisticsRes(const bool res)
    {
        logisticsRes = res;
    }
    void setHasBottom(const bool res)
    {
        hasBottom = res;
    }

    std::string &getQrCodeRes()
    {
        return qrCodeRes;
    }
    std::string &getLogisticsGT1()
    {
        return logistics1;
    }
    std::string &getLogisticsGT2()
    {
        return logistics2;
    }
    std::string &getBatchNum()
    {
        return batchNum;
    }
    std::string &getFormulaName()
    {
        return formulaName;
    }
    std::string &getLocateImageName()
    {
        return locateImageName;
    }
    std::string &getLocateCheckImageName()
    {
        return locateCheckImageName;
    }
    std::string &getCodeCheckImageName()
    {
        return codeCheckImageName;
    }
    cv::Mat *getLocateImage()
    {
        return locateImage;
    }
    cv::Mat *getLocateCheckImage()
    {
        return locateCheckImage;
    }
    cv::Mat *getCodeCheckImage()
    {
        return codeCheckImage;
    }
    float getLocateRes()
    {
        return locateRes;
    }
    bool getLocateCheckRes()
    {
        return locateCheckRes;
    }
    bool getLogisticsRes()
    {
        return logisticsRes;
    }
    bool getHasBottom()
    {
        return hasBottom;
    }

  private:
    std::string qrCodeRes;               // 二维码读码结果
    std::string logistics1;              // 物流码真实值1
    std::string logistics2;              // 物流码真实值2
    std::string logisticsPredict;        // 物流码预测值
    std::string batchNum;                // 批号
    std::string formulaName;             // 配方名
    std::string locateImageName;         // 定位图像
    std::string locateCheckImageName;    // 定位复核图像
    std::string codeCheckImageName;      // 打码复核图像
    float locateRes;                     // 定位结果
    bool locateCheckRes;                 // 定位复核结果
    bool logisticsRes;                   // 物流码比较结果
    std::atomic_bool hasBottom = true;   // 是否有瓶
    cv::Mat *locateImage = nullptr;      // 定位图像
    cv::Mat *locateCheckImage = nullptr; // 定位复核图像
    cv::Mat *codeCheckImage = nullptr;   // 打码复核图像
};
