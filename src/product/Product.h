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
    void setLocateImage(const std::string &str)
    {
        locateImage = str;
    }
    void setLocateCheckImage(const std::string &str)
    {
        locateCheckImage = str;
    }
    void setCodeCheckImage(const std::string &str)
    {
        codeCheckImage = str;
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
    std::string &getLocateImage()
    {
        return locateImage;
    }
    std::string &getLocateCheckImage()
    {
        return locateCheckImage;
    }
    std::string &getCodeCheckImage()
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
    std::string qrCodeRes;             // 二维码读码结果
    std::string logistics1;            // 物流码真实值1
    std::string logistics2;            // 物流码真实值2
    std::string logisticsPredict;      // 物流码预测值
    std::string batchNum;              // 批号
    std::string formulaName;           // 配方名
    std::string locateImage;           // 定位图像
    std::string locateCheckImage;      // 定位复核图像
    std::string codeCheckImage;        // 打码复核图像
    float locateRes;                   // 定位结果
    bool locateCheckRes;               // 定位复核结果
    bool logisticsRes;                 // 物流码比较结果
    std::atomic_bool hasBottom = true; // 是否有瓶
};
