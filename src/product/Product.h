#pragma once
#include "Logger.h"
#include "Utils.h"
#include <iostream>

struct Product
{
    bool locateCheckRes;               // 定位复核结果
    bool logisticsRes;                 // 物流码比较结果
    bool logisticsFalseFlag = false;   // 物流码是否获取失败
    bool locateFalseFlag = false;      // 定位是否失败
    bool locateCheckFalseFlag = false; // 定位复核是否失败
    bool isCode = false;               // 是否打码
    std::atomic_bool hasBottom = true; // 是否有瓶
    float locateRes;                   // 定位结果
    std::string qrCodeRes;             // 二维码读码结果
    std::string logistics1;            // 物流码真实值1
    std::string logistics2;            // 物流码真实值2
    std::string logisticsPredict;      // 物流码预测值
    std::string batchNum;              // 批号
    std::string formulaName;           // 配方名
    std::string locateImageName;       // 定位图像
    std::string locateCheckImageName;  // 定位复核图像
    std::string codeCheckImageName;    // 打码复核图像
    cv::Mat locateImage;               // 定位图像
    cv::Mat locateCheckImage;          // 定位复核图像
    cv::Mat codeCheckImage;            // 打码复核图像
};
