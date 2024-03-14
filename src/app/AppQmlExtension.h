#pragma once
#include "NonCopyable.h"
#include <string>
namespace AppFrame
{
class AppQmlExtension : public NonCopyable
{
  public:
    // 统一调用 接口区域
    static std::string insertSqlData(const std::string &data = ""); // 增
    static std::string deleteSqlData(const std::string &data = ""); // 删
    static std::string selectSqlData(const std::string &data = ""); // 查
    static std::string modifySqlData(const std::string &data = ""); // 改
    static std::string readPLC(const std::string &data = "");
    static std::string writePLC(const std::string &data = "");
    static std::string autoLogin(const std::string &data = "");
    static std::string zeroClearing(const std::string &data = "");
    static std::string refreshMainPage(const std::string &data = "");
    static std::string refreshPowerPage(const std::string &data = "");
    static std::string produceReset(const std::string &data = "");  // 复位
    static std::string selectProduct(const std::string &data = ""); // 更新最新瓶子数据
    static std::string autoLoad(const std::string &data = "");      // 配方自动下载

    static std::string selectPage(const std::string &data = "");   // 根据选择页数刷新界面
    static std::string initPageData(const std::string &data = ""); // 初始化页面信息
    static std::string powerOff(const std::string &data = "");
    static std::string getPLCCount(const std::string &data = "");
    static std::string getDataReport(const std::string &data = "");
    inline static bool isLogin_ = false;
};
} // namespace AppFrame