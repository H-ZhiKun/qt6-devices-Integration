#include "AppFrameworkImpl.h"
#include "AppFramework.h"
#include "AppMetaFlash.h"
#include "AppPlcExtension.h"
#include "AppProductExtension.h"
#include "AppQmlExtension.h"
#include "AppSourceExtension.h"
#include "AppStatisticsExtension.h"
#include "AppTest.h"
#include "BaseStatistics.h"
#include "CameraManager.h"
#include "CapDevice.h"
#include "CapProduct.h"
#include "CapStatistics.h"
#include "CircleDevice.h"
#include "CircleProduct.h"
#include "Cognex.h"
#include "ImagePainter.h"
#include "LineDevice.h"
#include "LineProduct.h"
#include "LineStatistics.h"
#include "Logger.h"
#include "Permission.h"
#include "PgsqlHelper.h"
#include "PrinterDevice.h"
#include "Utils.h"
#include "WebManager.h"
#include <memory>

using namespace AppFrame;

AppFramework &AppFramework::instance()
{
    return AppFrameworkImpl::instance();
}

AppFrame::AppFrameworkImpl::AppFrameworkImpl()
{
}

AppFrame::AppFrameworkImpl::~AppFrameworkImpl() noexcept
{
}

int AppFrame::AppFrameworkImpl::run(QQmlApplicationEngine *engine)
{
    // 软件开启前的初始化 顺序不可修改
    loadConfig();    // 1 加载配置文件
    initLogger();    // 2 初始化日志模块
    initSqlHelper(); // 3 数据库初始化
    LogInfo("init start SqlHelper");
    initNetworkClient(); // 4 网络客户端初始化
    initStatistics();
    LogInfo("init start NetworkClient");
    initCameraManager(); // 5 相机初始化
    LogInfo("init start CameraManager");
    initPLC(); // 6 PLC初始化
    LogInfo("init start PLC");
    initProduct(); // 7 产品初始化
    LogInfo("init start Product");
    initExpectation(); // 8 初始化qml扩展调用模块
    LogInfo("init start Expectation");
    engineLoad(engine); // 9 qml界面加载
    LogInfo("init start qml");
    afterInit(); // 初始化完成之后行为
    return 0;
}

QQmlApplicationEngine *AppFrame::AppFrameworkImpl::getEngine()
{
    return engine_;
}

std::string AppFrame::AppFrameworkImpl::expected(const ExpectedFunction &expectedType, const std::string &jsValue)
{
    // 使用 find 查找迭代器
    auto it = mapExpectedFunction_.find(expectedType);

    // 如果找到，直接调用对应函数
    if (it != mapExpectedFunction_.end())
    {
        return it->second(jsValue);
    }

    // 否则返回默认的错误信息
    return Utils::makeResponse(false, "function not found");
}

bool AppFrame::AppFrameworkImpl::registerExpectation(const ExpectedFunction &expectedType,
                                                     std::function<std::string(const std::string &)> &&api)
{
    // 使用结构化绑定和emplace优化插入
    auto [it, inserted] = mapExpectedFunction_.emplace(expectedType, std::move(api));
    // 返回插入是否成功
    return inserted;
}

void AppFrame::AppFrameworkImpl::initExpectation()
{
    registerExpectation(ExpectedFunction::DeleteSqlData, &AppQmlExtension::deleteSqlData);
    registerExpectation(ExpectedFunction::SelectSqlData, &AppQmlExtension::selectSqlData);
    registerExpectation(ExpectedFunction::ModifySqlData, &AppQmlExtension::modifySqlData);
    registerExpectation(ExpectedFunction::InsertSqlData, &AppQmlExtension::insertSqlData);
    registerExpectation(ExpectedFunction::ReadPLC, &AppQmlExtension::readPLC);
    registerExpectation(ExpectedFunction::WritePLC, &AppQmlExtension::writePLC);
    registerExpectation(ExpectedFunction::RefreshMainPage, &AppQmlExtension::refreshMainPage);
    registerExpectation(ExpectedFunction::RefreshPowerPage, &AppQmlExtension::refreshPowerPage);
    registerExpectation(ExpectedFunction::ZeroClearing, &AppQmlExtension::zeroClearing);
    registerExpectation(ExpectedFunction::AutoLogin, &AppQmlExtension::autoLogin);
    registerExpectation(ExpectedFunction::ProduceReset, AppQmlExtension::produceReset);
    registerExpectation(ExpectedFunction::SelectProduct, &AppQmlExtension::selectProduct);
    registerExpectation(ExpectedFunction::AutoLoad, &AppQmlExtension::autoLoad);
    registerExpectation(ExpectedFunction::InitPageData, &AppQmlExtension::initPageData);
    registerExpectation(ExpectedFunction::PowerOff, &AppQmlExtension::powerOff);
    registerExpectation(ExpectedFunction::GetPLCCount, &AppQmlExtension::getPLCCount);
    registerExpectation(ExpectedFunction::GetDataReport, &AppQmlExtension::getDataReport);
}

void AppFrame::AppFrameworkImpl::quitProgram()
{
    // 程序关闭
    QCoreApplication::quit();
}

void AppFrame::AppFrameworkImpl::loadConfig()
{
    try
    {
        std::string filePath = qApp->applicationDirPath().toStdString() + "/config.yaml";
        config_ = YAML::LoadFile(filePath);
    }
    catch (const YAML::Exception &e)
    {
        Utils::appExit(-1);
    }
}

void AppFrame::AppFrameworkImpl::initLogger()
{
    // 初始化日志记录器
    std::string logPath = qApp->applicationDirPath().toStdString() + std::string("/logs/") +
                          config_["app"]["log"]["log_file_name"].as<std::string>();
    size_t logLevel = config_["app"]["log"]["log_level"].as<size_t>();
    size_t logFileCount = config_["app"]["log"]["log_file_count"].as<size_t>();
    size_t logFileSize = Utils::anyFromString<size_t>(config_["app"]["log"]["log_file_size"].as<std::string>());
    CLogger::GetLogger().initLogger(logPath, logLevel, logFileSize, logFileCount);
    // appTest().test_s7Client();
}

void AppFrame::AppFrameworkImpl::initSqlHelper()
{
    std::string host = config_["app"]["database"]["host"].as<std::string>();
    uint16_t port = config_["app"]["database"]["port"].as<uint16_t>();
    std::string dbName = config_["app"]["database"]["dbname"].as<std::string>();
    std::string user = config_["app"]["database"]["user"].as<std::string>();
    std::string pwd = "~!dtfs@#";
    if (!PgsqlHelper::getSqlHelper().initSqlHelper(host, port, dbName, user, pwd))
    {
        LogInfo("sqlhelper init failed.");
        Utils::appExit(-1);
    }
}

void AppFrame::AppFrameworkImpl::initNetworkClient()
{
    std::string strType = config_["product"]["type"].as<std::string>();
    std::string cognexIp = config_["cognex"]["host"].as<std::string>();
    uint16_t cognexPort = config_["cognex"]["port"].as<uint16_t>();
    std::string permissionIp = config_["permission"]["host"].as<std::string>();
    uint16_t permissionPort = config_["permission"]["port"].as<uint16_t>();
    std::string painterIp = config_["painter"]["host"].as<std::string>();
    uint16_t painterPort = config_["painter"]["port"].as<uint16_t>();

    registerPlugin<Cognex>(std::make_unique<Cognex>());
    const auto &cognex = getPlugin<Cognex>();
    cognex->startClient(cognexIp.c_str(), cognexPort);

    registerPlugin<Permission>(std::make_unique<Permission>());
    const auto &permission = getPlugin<Permission>();
    permission->startClient(permissionIp.c_str(), permissionPort);

    registerPlugin<WebManager>(std::make_unique<WebManager>());
    const auto &webManager = getPlugin<WebManager>();
    webManager->init(config_);

    registerPlugin<PrinterDevice>(std::make_unique<PrinterDevice>());
    const auto &printer = getPlugin<PrinterDevice>();
    if (strType != "cap")
    {
        printer->startClient(painterIp.c_str(), painterPort);
    }

    QObject::connect(printer, &PrinterDevice::printSend,
                     [this]() { AppStatisticsExtension::afterCountChanged(AppPLCCountType::print_send); });
    QObject::connect(printer, &PrinterDevice::printComplete,
                     [this]() { AppStatisticsExtension::afterCountChanged(AppPLCCountType::print_right); });

    QObject::connect(
        webManager, &WebManager::algoRight,
        [this](const ALGOType modelType, const uint32_t bottleNum, const std::string result, const std::string area) {
            AppProductionExtension::afterALGORecv(modelType, bottleNum, result, area);
        });

    // 获取到二维码并发送
    QObject::connect(cognex, &Cognex::readQRCode, [this](uint32_t number, const std::string value) {
        AppProductionExtension::afterCognexRecv(number, value);
        if (value == "no read")
        {
            AppStatisticsExtension::afterCountChanged(AppPLCCountType::code_get);
        }
        else
        {
            AppStatisticsExtension::afterCountChanged(AppPLCCountType::code_right);
        }
    });
    // 获取到物流码并存储
    QObject::connect(permission, &Permission::codeRight, [this](uint32_t number, const std::string code) {
#ifndef APP_TEST
        AppProductionExtension::afterPermissionRecv(number, code);
#endif
        AppStatisticsExtension::afterCountChanged(AppPLCCountType::code_feedback);
    });
}
void AppFrame::AppFrameworkImpl::initStatistics()
{
    std::string strType = config_["product"]["type"].as<std::string>();
    if (strType == "circle")
    {
        // registerPlugin<BaseStatistics>(std::make_unique<BaseStatistics>());
    }
    else if (strType == "line")
    {
        registerPlugin<BaseStatistics>(std::make_unique<LineStatistics>());
    }
    else if (strType == "cap")
    {
        registerPlugin<BaseStatistics>(std::make_unique<CapStatistics>());
    }
}

void AppFrame::AppFrameworkImpl::initPLC()
{

    std::string strType = config_["product"]["type"].as<std::string>();
    using PLCRegistrationFunction = std::function<void()>;

    const std::unordered_map<std::string, PLCRegistrationFunction> plcRegistrationMap = {
        {"circle", [this]() { registerAndInitPLC<CircleDevice>(); }},
        {"line", [this]() { registerAndInitPLC<LineDevice>(); }},
        {"cap", [this]() { registerAndInitPLC<CapDevice>(); }},

    };
    auto it = plcRegistrationMap.find(strType);
    if (it != plcRegistrationMap.end())
    {
        it->second();
    }
    else
    {
        LogError("Unsupported product type for PLC initialization");
        Utils::appExit(-1);
    }

    const auto &plc = getPlugin<BasePLCDevice>();
    QObject::connect(plc, &BasePLCDevice::signalStatus,
                     [this](const uint8_t status) { AppPlcExtension::afterPLCStatesChange(status); });
    QObject::connect(plc, &BasePLCDevice::signalQR,
                     [this]() { AppStatisticsExtension::afterCountChanged(AppPLCCountType::code_trigger); });
    QObject::connect(plc, &BasePLCDevice::signalOCR,
                     [this]() { AppStatisticsExtension::afterCountChanged(AppPLCCountType::ocr_trigger); });
    QObject::connect(plc, &BasePLCDevice::signalLocate,
                     [this]() { AppStatisticsExtension::afterCountChanged(AppPLCCountType::locate_trigger); });
    QObject::connect(plc, &BasePLCDevice::signalCheck,
                     [this]() { AppStatisticsExtension::afterCountChanged(AppPLCCountType::check_trigger); });

    QObject::connect(plc, &BasePLCDevice::signalProductIn, [this](const uint16_t count) {
        AppProductionExtension::afterProductIn(count);
        AppStatisticsExtension::afterCountChanged(AppPLCCountType::amount);
    });
    QObject::connect(plc, &BasePLCDevice::signalProductOut, [this](const uint16_t count) {
        AppProductionExtension::afterProductOut(count);
        AppStatisticsExtension::afterCountChanged(AppPLCCountType::remove);
    });
}

void AppFrame::AppFrameworkImpl::initCameraManager()
{
    registerPlugin<CameraManager>(std::make_unique<CameraManager>());
    const auto &cameraManager = getPlugin<CameraManager>();
    cameraManager->start(config_);
    uint16_t image_normal_time = config_["app"]["resource"]["image_normal_time"].as<uint16_t>();
    uint16_t image_ng_time = config_["app"]["resource"]["image_ng_time"].as<uint16_t>();
    AppSourceExtension::initFilePath(qApp->applicationDirPath());
    AppSourceExtension::deleteImage(image_normal_time, image_ng_time, qApp->applicationDirPath());
    QObject::connect(cameraManager, &CameraManager::caputureImage,
                     [this](const std::string cameraType, int32_t imgNumber, const cv::Mat image) {
                         AppProductionExtension::afterCaputureImage(cameraType, imgNumber, image);
                         if (cameraType == "Location")
                             AppStatisticsExtension::afterCountChanged(AppPLCCountType::locate_get);
                         else if (cameraType == "OCR")
                             AppStatisticsExtension::afterCountChanged(AppPLCCountType::ocr_get);
                         else if (cameraType == "LocateCheck")
                             AppStatisticsExtension::afterCountChanged(AppPLCCountType::check_get);
                     });
}

void AppFrame::AppFrameworkImpl::initProduct()
{
    std::string strType = config_["product"]["type"].as<std::string>();

    using ProductRegistrationFunction = std::function<void()>;
    const std::unordered_map<std::string, ProductRegistrationFunction> productRegistrationMap = {
        {"circle", [this]() { registerAndInitProduct<CircleProduct>(config_); }},
        {"line", [this]() { registerAndInitProduct<LineProduct>(config_); }},
        {"cap", [this]() { registerAndInitProduct<CapProduct>(config_); }},
        // 更多其他初始化函数
    };
    auto it = productRegistrationMap.find(strType);
    if (it != productRegistrationMap.end())
    {
        it->second();
    }
    else
    {
        LogError("Unsupported product type in config");
        Utils::appExit(-1);
    }
}

void AppFrame::AppFrameworkImpl::afterInit()
{
    // AppQmlExtension::produceReset(); // 此时有可能未连接上
}

void AppFrame::AppFrameworkImpl::engineLoad(QQmlApplicationEngine *engine)
{
    const std::string strType = config_["product"]["type"].as<std::string>();

    // Define a mapping for product type to title name and module name
    const std::unordered_map<std::string, std::pair<QString, QString>> productTypeMap = {
        {"line", {"尖庄100毫升光瓶瓶盖数据采集激光打码检测设备", "MainWindow"}},
        {"circle", {"尖庄500毫升光瓶瓶盖数据采集激光打码检测设备", "MainWindow"}},
        {"cap", {"尖庄100毫升光瓶瓶盖在线检测设备", "MainWindow"}}
        // Add more mappings as needed
    };

    auto it = productTypeMap.find(strType);
    if (it != productTypeMap.end())
    {
        const auto &[titleName, moduleName] = it->second;

        // 注册MyObject类
        qmlRegisterType<ImagePainter>("AppFrame", 1, 0, "ImagePainter");
        engine->rootContext()->setContextProperty("appMetaFlash", &appMetaFlash());
        engine->rootContext()->setContextProperty("iconDir",
                                                  "file:///" + qApp->applicationDirPath() + "/ico/"); // 获取当前路径
        engine->rootContext()->setContextProperty("pname", titleName);
        engine->rootContext()->setContextProperty("deviceName", strType.c_str());
        engine->loadFromModule("DeviceIntegration", moduleName);
        engine_ = engine;
    }
    else
    {
        LogError("product type in config error so engine loadFromModule failed");
        Utils::appExit(-1);
    }
}