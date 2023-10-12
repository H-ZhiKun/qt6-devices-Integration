#pragma once
#include "AppFramework.h"
#include "BasePLCDevice.h"
#include "BaseProduct.h"
#include "BaumerManager.h"
#include "CapDevice.h"
#include "CapProduct.h"
#include "Cognex.h"
#include "Domino.h"
#include "LineProduct.h"
#include "Logger.h"
#include "Permission.h"
#include "PgsqlHelper.h"
#include "WebManager.h"
#include <QBrush>
#include <QDir>
#include <QPainter>
#include <QPen>
#include <QQmlApplicationEngine>
#include <memory>
#include <mutex>
#include <opencv2/opencv.hpp>
#include <thread>
#include <yaml-cpp/yaml.h>
namespace AppFrame
{

class AppFrameworkImpl final : public AppFramework
{
  public:
    AppFrameworkImpl();
    ~AppFrameworkImpl() noexcept override;
    inline static AppFrameworkImpl &instance()
    {
        static AppFrameworkImpl instance;
        return instance;
    }
    virtual int run(QQmlApplicationEngine *engine) override;
    virtual std::string expected(const ExpectedFunction &expectedType, const std::string &jsValue) override;
    virtual bool registerExpectation(const ExpectedFunction &expectedType,
                                     std::function<std::string(const std::string &)> &&api) override;

  protected:
    // 统一调用 接口区域
    std::string dominoConnect(const std::string &);
    std::string deleteFormula(const std::string &);
    std::string modifyFormula(const std::string &);
    std::string insertFormula(const std::string &);
    std::string selectFormula(const std::string &);
    std::string getCameraParam(const std::string &);
    std::string setCameraParam(const std::string &);
    std::string insertUser(const std::string &);
    std::string selectUserID(const std::string &);
    std::string selectUser(const std::string &);
    std::string deleteUser(const std::string &);
    std::string modifyUser(const std::string &);
    std::string selectAlert(const std::string &);
    std::string collectImage(const std::string &);
    std::string readPLC(const std::string &);
    std::string writePLC(const std::string &);
    std::string refreshMainPage(const std::string &);
    std::string refreshStrightMainPage(const std::string &);
    std::string refreshPowerPage(const std::string &);
    std::string zeroClearing(const std::string &);
    std::string strightZeroClearing(const std::string &);

    std::string refreshElecData(const std::string &); // 刷新直线式电能表数据

    // 差异调用 接口区域
    void storeImagePainter(QQmlApplicationEngine *engine);
    void loadConfig();
    void saveConfig();
    void initLogger();
    void initSqlHelper();
    void initNetworkClient();
    void initPLC();
    void memoryClean();
    void initBaumerManager();
    void initFile();

    void initProduct();
    void updateAlertData();   // 更新报警信息
    void updateFormulaData(); // 初始化配方界面
    void updateUserData();
    void timerTask(); // 定时任务

    void whenSiganlQR(const uint64_t number);
    void whenSignalCoding();
    void whenSignaOCR();
    void whenSignaRemove();

    void afterCognexRecv(const std::string &code);
    void afterPermissionRecv(const std::string &num, const std::string &code1, const std::string &code2);
    void afterCaputureImage(const uint8_t &windId, const cv::Mat &mat);

    void processOCR(const std::string &);
    void processTangle(const std::string &);
    void processCheck(const std::string &);

    void sendOneToAlgo(); // 初始化服务端的python模型
    void drawText(QImage &img, const QString &text);
    void drawOcrRes(QImage &img, OcrRes &ocr);

  private:
    // 私有变量区域
    bool bInitComplete = false;
    // 程序路径 begin
    std::string strAppPath_;
    std::string strTanglePath_;
    std::string strTangleCheckPath_;
    std::string strOcrPath_;
    std::string strTangleResultPath_;
    std::string strTangleCheckResultPath_;
    std::string strOcrResultPath_;
    // 程序路径 end
    YAML::Node config_;
    std::list<std::thread> lvFulltimeThread_;
    std::atomic_bool bThreadHolder{true};
    std::atomic_bool saveImageFlag;
    std::unordered_map<ExpectedFunction, std::function<std::string(const std::string &)>> mapExpectedFunction_;
    BaseProduct *product_{nullptr};
    // Module 组装区域
    // tcp client begin
    Domino *domino_ = nullptr;
    Cognex *cognex_ = nullptr;
    Permission *permission_ = nullptr;
    WebManager *webManager_ = nullptr;
    // tcp client end
    BasePLCDevice *plcDev_ = nullptr;
    BaumerManager *baumerManager_ = nullptr;
    ProductData *productData_ = nullptr;
    std::unordered_map<uint8_t, QObject *> mapStorePainter_;   // 初始化存放所有qml中的painter对象
    std::unordered_map<std::string, uint8_t> mapWindId2Index_; // 存放窗口名到窗口对象序号的映射
  public:
    // 调用qml 对象函数工具

    // 调用C++ 对象函数工具
    bool invokeCpp(QObject *object, const char *functionName, const QVariantList &args)
    {
        bool success = false;
        if (object)
        {
            success = QMetaObject::invokeMethod(object, functionName, Qt::AutoConnection, args);
            if (!success)
                LogError("method={} is called failed", functionName);
        }
        else
        {
            LogError("object is nullptr");
        }
        return success;
    }
    template <typename... Args> bool invokeCpp(QObject *object, const char *functionName, Args &&...arguments)
    {
        bool success = false;
        if (object)
        {
            success =
                QMetaObject::invokeMethod(object, functionName, Qt::AutoConnection, std::forward<Args>(arguments)...);
            if (success == false)
                LogError("method={} is called failed", functionName);
        }
        else
        {
            LogError("object is nullptr");
        }
        return success;
    }
};
} // namespace AppFrame
