#pragma once
#include "NonCopyable.h"
#include <QQmlApplicationEngine>
#include <functional>
#include <string>

namespace AppFrame
{

enum class ExpectedFunction
{
    DominoConnect = 0,          // 多米诺设备连接
    DeleteFormula = 1,          // 删除某个配方数据
    SelectFormula = 2,          // 查询某个配方数据
    ModifyFormula = 3,          // 修改某个配方数据
    InsertFormula = 4,          // 插入某个配方数据
    GetCameraParam = 5,         // 查询某个相机数据
    SetCameraParam = 6,         // 修改相机数据
    InsertUser = 8,             // 新建用户
    SelectUserID = 9,           // 查询用户ID
    SelectUser = 10,            // 查询用户
    DeleteUser = 11,            // 删除用户
    ModifyUser = 12,            // 修改用户
    SelectAlert = 13,           // 选择报警信息
    CollectImage = 14,          // 采集图像
    ReadPLC = 15,               // 读PLC数据
    WritePLC = 16,              // 写PLC数据
    RefreshMainPage = 17,       // 刷新主页面数据
    RefreshPowerPage = 18,      // 刷新电能数据
    RefreshElecData = 19,       // 刷新直线式电能表数据
    RefreshStrightMainPage = 20 // 刷新主页面数据
};
class AppFramework : public NonCopyable
{
  public:
    virtual ~AppFramework() = default;
    /**
     * @brief 获取应用程序框架的唯一实例
     * @details 该函数返回应用程序框架的唯一实例，是单例模式的实现。该函数不接收任何参数。
     * @return 返回AppFramework类型的引用，表示应用程序框架的唯一实例。
     */
    static AppFramework &instance();
    virtual int run(QQmlApplicationEngine *) = 0;
    virtual std::string expected(const ExpectedFunction &expectedType, const std::string &jsValue) = 0;
    virtual bool registerExpectation(const ExpectedFunction &expectedType,
                                     std::function<std::string(const std::string &)> &&api) = 0;
};
inline AppFramework &appFramework()
{
    return AppFramework::instance();
}
} // namespace AppFrame
