#pragma once
#include "cryptopp/aes.h"
#include "cryptopp/base64.h"
#include "cryptopp/cryptlib.h"
#include "cryptopp/filters.h"
#include "cryptopp/modes.h"
#include "cryptopp/osrng.h"
#include <QBuffer>
#include <QByteArray>
#include <QImage>
#include <chrono>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <json/json.h>
#include <opencv2/opencv.hpp>
#include <sstream>
#include <string>
#include <vector>
class Utils
{
  public:
    /**
     * @brief 对传入的字符串进行Base64编码
     * @param str 需要进行Base64编码的字符串指针
     * @param bytes 字符串的长度（单位：字节）
     * @return 返回Base64编码后的字符串
     */
    static std::string base64Encode(const unsigned char *str, int bytes);
    /**
     * @brief 对传入的Base64字符串进行解码
     * @param str 需要进行解码的Base64字符串指针
     * @param length Base64字符串的长度（单位：字符）
     * @return 返回解码后的原始字符串
     */
    static std::string base64Decode(const char *str, int length);
    /**
     * @brief 将字符串时间表示转换为 std::chrono::system_clock::time_point
     *
     * @param timeString 要转换的字符串时间表示
     * @return std::chrono::system_clock::time_point 表示时间的时间点
     */
    static std::chrono::system_clock::time_point stringToTimePoint(const std::string &timeString);

    /**
     * @brief 获取当前系统时间并返回字符串表示
     * @return 当前系统时间的字符串表示
     */
    static std::string getCurrentTime(bool hasMillisecond = false);
    static void getCurrentTime(std::string &year, std::string &month, std::string &day, std::string &hour,
                               std::string &minute, std::string &second);
    /**
     * @brief 获取当前时间的任意前后秒数的时间点的字符串表示
     *
     * @param seconds 要获取的时间点相对于当前时间的秒数偏移量 正数是未来时间，负数是过去时间
     * @param currentTime
     * @return std::string 表示所获取时间点的字符串表示
     */
    static std::string getTimeOffset(int seconds, const std::string &currentTime = "");

    /**
     * @brief 图片数据转换base64字符串
     *
     * @param filePath 文件路径或者
     * @return 返回base64字符串
     */
    static std::string imgToBase64(const std::string &filePath);
    /**
     * @brief 图片数据转换base64字符串
     *
     * @param img 图片对象
     * @return 返回base64字符串
     */
    static std::string imgToBase64(const QImage &img);
    /**
     * @brief 将JSON格式的字符串转换为Json::Value对象
     * @param jsString JSON格式的字符串
     * @return 返回转换后的Json::Value对象
     */
    static Json::Value stringToJson(const std::string &jsString);
    /**
     * @brief 将Json::Value对象转换为JSON格式的字符串
     * @param jsVal Json::Value对象
     * @return 返回转换后的JSON格式的字符串
     */
    static std::string jsonToString(const Json::Value &jsVal);
    /**
     * @brief 从指定路径的JSON文件中读取数据并转换为Json::Value对象
     * @param filePath JSON文件的完整路径
     * @return 返回读取并转换后的Json::Value对象
     */
    static Json::Value readJsonFile(const std::string &filePath);
    /**
     * @brief 将Json::Value对象写入指定路径的JSON文件中
     * @param filePath JSON文件的完整路径
     * @param jsVal 需要写入文件的Json::Value对象
     * @return 返回写入是否成功的结果，成功返回true，否则返回false
     */
    static bool writeJsonFile(const std::string &filePath, const Json::Value &jsVal);

    /**
     * @brief 目录遍历，查找所有文件
     * @param path 目录路径
     * @return std::vector<std::string> 包含目录下所有文件的向量
     */
    static std::vector<std::filesystem::path> findAllFiles(const std::filesystem::path &path);
    /**
     * @brief 删除过期文件
     * @param currentTime 当前时间
     * @param path 文件路徑
     * @param offsetDay 过期天数  过期日期 = 当前日期 + 过期天数(负数过去时间)
     * @return bool 是否成功删除
     */
    static bool removeOutdatedFiles(const std::string &currentTime, const std::string &path, int offsetDay);

    static void asyncTask(std::function<void(void)> &&task);

    static void appExit(int exitCode = 0);

    static QImage matToQImage(const cv::Mat &matData);

    static std::string makeResponse(bool bOk = true, Json::Value &&details = {}, std::string &&description = "");
    /**
     * @description: 使用aes给字符串加密
     * @param {string} &plain 需要加密的字符串
     * @return {string} 返回加密的字符串
     */
    static std::string encrytByAES(const std::string &plain);
    /**
     * @description: 使用aes给字符串解密
     * @param {string} &encode 需要解密的字符串
     * @return {string} 返回解密的字符串
     */
    static std::string decrytByAES(const std::string &encode);

    template <typename T> static T anyFromString(const std::string &str)
    {
        T value;
        std::stringstream ss(str);
        ss >> value;
        return value;
    }

    static cv::Mat qImageToMat(QImage &qim);
    static std::vector<std::string> splitString(const std::string &input, const std::string &delimiter);
    static std::string makeHttpBodyWithCVMat(const cv::Mat &algoImage, const uint16_t bottomNum,
                                             const std::string &imageName, const std::string &modelName);
    static std::string compressMatToZlib(const cv::Mat &inputMat);
};
