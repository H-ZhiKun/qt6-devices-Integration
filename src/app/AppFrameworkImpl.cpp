#include "AppFrameworkImpl.h"
#include "AppCentralExtension.h"
#include "AppFramework.h"
#include "AppMetaFlash.h"
#include "AppPlcExtension.h"
#include "AppProductExtension.h"
#include "AppQmlExtension.h"
#include "AppSourceExtension.h"
#include "AppStatisticsExtension.h"
#include "AppUdpCameraExtension.h"
#include "BasePLCDevice.h"
#include "BaseStatistics.h"
#include "BoxAppDevice.h"
#include "BoxAppProduct.h"
#include "BoxAppStatistics.h"
#include "CameraManager.h"
#include "CapDevice.h"
#include "CapProduct.h"
#include "CapStatistics.h"
#include "CentralClient.h"
#include "CircleDevice.h"
#include "CircleProduct.h"
#include "CircleStatistics.h"
#include "Cognex.h"
#include "ExtraTest.h"
#include "ImagePainter.h"
#include "ImpurityDevice.h"
#include "ImpurityProduct.h"
#include "ImpurityStatistics.h"
#include "LineDevice.h"
#include "LineProduct.h"
#include "LineStatistics.h"
#include "Logger.h"
#include "Permission.h"
#include "PgsqlHelper.h"
#include "PrinterDevice.h"
#include "UDPCamera.h"
#include "Utils.h"
#include "WebManager.h"
#include <Edge.h>
#include <QQmlContext>
#include <qdebug.h>

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
    beforClose();
}

int AppFrame::AppFrameworkImpl::run(QQmlApplicationEngine *engine)
{
    // 软件开启前的初始化 顺序不可修改
    initAsyncTask();
    initMetaFlash();
    loadConfig(); // 1 加载配置文件
    initFilePath();
    initLogger();    // 2 初始化日志模块
    initSqlHelper(); // 3 数据库初始化
    initExtraTest();
    LogInfo("init start SqlHelper");
    initNetworkClient(); // 4 网络客户端初始化
    initNetworkServer(); // 5 服务端初始化
    initStatistics();
    LogInfo("init start NetworkClient");
    initCameraManager(); // 6 相机初始化
    LogInfo("init start CameraManager");
    initPLC(); // 7 PLC初始化
    LogInfo("init start PLC");
    initProduct(); // 8 产品初始化
    LogInfo("init start Product");
    initExpectation(); // 9 初始化qml扩展调用模块
    LogInfo("init start Expectation");
    engineLoad(engine); // 10 qml界面加载
    LogInfo("init start qml");
    afterInit(); // 初始化完成之后行为
    return 0;
}

QQmlApplicationEngine *AppFrame::AppFrameworkImpl::getEngine()
{
    return engine_;
}
QObject *AppFrame::AppFrameworkImpl::getMeta()
{
    return appMeta_.get();
}
void AppFrame::AppFrameworkImpl::runAfter(std::string &&taskName, std::function<void(void)> &&task,
                                          uint32_t milliseconds)
{
    taskActuators_->runAfter(std::move(taskName), std::move(task), milliseconds);
}
void AppFrame::AppFrameworkImpl::runEvery(std::string &&taskName, std::function<void(void)> &&task,
                                          uint32_t milliseconds)
{
    taskActuators_->runEvery(std::move(taskName), std::move(task), milliseconds);
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
void initUdpCamera()
{
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
    registerExpectation(ExpectedFunction::FuncTest, &AppQmlExtension::funcTest);
    // registerExpectation(ExpectedFunction::EleSwitch, &AppQmlExtension::removeTest);
    registerExpectation(ExpectedFunction::CollectTest, &AppQmlExtension::collectTest);
}

void AppFrame::AppFrameworkImpl::quitProgram()
{
    // 程序关闭
    QCoreApplication::quit();
}
void AppFrame::AppFrameworkImpl::stop()
{
    taskActuators_->stop();
    mapExpectedFunction_.clear();
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
void AppFrame::AppFrameworkImpl::initFilePath()
{
    uint16_t image_normal_time = config_["app"]["resource"]["image_normal_time"].as<uint16_t>();
    uint16_t image_ng_time = config_["app"]["resource"]["image_ng_time"].as<uint16_t>();
    AppSourceExtension::initFilePath(qApp->applicationDirPath());
    AppSourceExtension::deleteImage(image_normal_time, image_ng_time, qApp->applicationDirPath());
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

void AppFrame::AppFrameworkImpl::initMetaFlash()
{
    appMeta_ = std::make_unique<AppMetaFlash>();
}
void AppFrame::AppFrameworkImpl::initAsyncTask()
{
    taskActuators_ = std::make_unique<ConcurrentPool>();
}

void AppFrame::AppFrameworkImpl::initExtraTest()
{
    registerPlugin<ExtraTest>(std::make_unique<ExtraTest>());
}

void AppFrame::AppFrameworkImpl::initNetworkClient()
{
    if (config_["cognex"])
    {
        std::string cognexIp = config_["cognex"]["host"].as<std::string>();
        uint16_t cognexPort = config_["cognex"]["port"].as<uint16_t>();
        registerPlugin<Cognex>(std::make_unique<Cognex>());
        const auto &cognex = getPlugin<Cognex>();
        cognex->startClient(cognexIp.c_str(), cognexPort);
        // 获取到二维码并发送
        QObject::connect(cognex, &Cognex::readQRCode, [this](uint32_t number, const std::string value) {
            AppProductionExtension::afterCognexRecv(number, value);
            AppStatisticsExtension::afterTriggerChanged(AppPLCCountType::code_get);
        });
    }

    if (config_["permission"])
    {
        std::string permissionIp = config_["permission"]["host"].as<std::string>();
        uint16_t permissionPort = config_["permission"]["port"].as<uint16_t>();
        registerPlugin<Permission>(std::make_unique<Permission>());
        const auto &permission = getPlugin<Permission>();
        permission->startClient(permissionIp.c_str(), permissionPort);
        // 获取到物流码并存储
        QObject::connect(permission, &Permission::codeRight, [this](uint32_t number, const std::string code) {
            const auto &test = appFramework().getPlugin<ExtraTest>();
            if (!test->isPrintTest())
            {
                AppProductionExtension::afterPermissionRecv(number, code);
            }
        });
    }

    if (config_["painter"])
    {
        std::string painterIp = config_["painter"]["host"].as<std::string>();
        uint16_t painterPort = config_["painter"]["port"].as<uint16_t>();

        registerPlugin<PrinterDevice>(std::make_unique<PrinterDevice>());
        const auto &printer = getPlugin<PrinterDevice>();
        printer->startClient(painterIp.c_str(), painterPort);
        QObject::connect(printer, &PrinterDevice::printSend,
                         [this]() { AppStatisticsExtension::afterTriggerChanged(AppPLCCountType::print_send); });
        QObject::connect(printer, &PrinterDevice::printComplete,
                         [this]() { AppStatisticsExtension::afterTriggerChanged(AppPLCCountType::print_right); });
    }

    if (config_["controlSystem"])
    {
        std::string controlSystemIp = config_["controlSystem"]["host"].as<std::string>();
        uint16_t controlSystemPort = config_["controlSystem"]["port"].as<uint16_t>();
        registerPlugin<CentralClient>(std::make_unique<CentralClient>());
        const auto &centralClient = getPlugin<CentralClient>();
        centralClient->startClient(controlSystemIp.c_str(), controlSystemPort);
    }

    if (config_["algorithm"])
    {
        registerPlugin<WebManager>(std::make_unique<WebManager>());
        const auto &webManager = getPlugin<WebManager>();
        webManager->init(config_);

        QObject::connect(webManager, &WebManager::algoRight,
                         [this](const ALGOType modelType, const uint32_t bottleNum, const std::string result,
                                const std::string area) {
                             AppProductionExtension::afterALGORecv(modelType, bottleNum, result, area);
                         });
    }
    if (config_["udpCamera"])
    {
        registerPlugin<UDPCamera>(std::make_unique<UDPCamera>());
        const auto &udp = getPlugin<UDPCamera>();
        udp->start();
        QObject::connect(udp, &UDPCamera::packetAssembled, [this](QString windName, QByteArray packet) {
            AppUdpCameraExtension::aftercameraTragger(windName, std::move(packet));
        });
    }
}

void AppFrame::AppFrameworkImpl::initNetworkServer()
{
    // 边缘设备信息收集
    if (config_["product"]["type"].as<std::string>() == "impurity")
    {
        registerPlugin<Edge>(std::make_unique<Edge>());
        const auto &edge = getPlugin<Edge>();
        edge->startServer(8188);
    }
}

void AppFrame::AppFrameworkImpl::initStatistics()
{
    std::string strType = config_["product"]["type"].as<std::string>();
    uint16_t ngMax = config_["product"]["ngMax"].as<uint16_t>();
    if (strType == "circle")
    {
        registerPlugin<BaseStatistics>(std::make_unique<CircleStatistics>());
    }
    else if (strType == "line")
    {
        registerPlugin<BaseStatistics>(std::make_unique<LineStatistics>());
    }
    else if (strType == "cap")
    {
        registerPlugin<BaseStatistics>(std::make_unique<CapStatistics>());
    }
    else if (strType == "impurity")
    {
        registerPlugin<BaseStatistics>(std::make_unique<ImpurityStatistics>());
    }
    else if (strType == "boxApp")
    {
        registerPlugin<BaseStatistics>(std::make_unique<BoxAppStatistics>());
    }
    const auto &statistics = getPlugin<BaseStatistics>();
    QObject::connect(
        statistics, &BaseStatistics::sendReport,
        [this](const std::unordered_map<ReportKey, int32_t> data) { AppCentralExtension::uploadData(data); });
    statistics->setNgMax(ngMax);
}

void AppFrame::AppFrameworkImpl::initPLC()
{
    if (config_["plc"])
    {
        std::string strType = config_["product"]["type"].as<std::string>();
        using PLCRegistrationFunction = std::function<void()>;

        const std::unordered_map<std::string, PLCRegistrationFunction> plcRegistrationMap = {
            {"circle", [this]() { registerAndInitPLC<CircleDevice>(); }},
            {"line", [this]() { registerAndInitPLC<LineDevice>(); }},
            {"cap", [this]() { registerAndInitPLC<CapDevice>(); }},
            {"impurity", [this]() { registerAndInitPLC<ImpurityDevice>(); }},
            {"boxApp", [this]() { registerAndInitPLC<BoxAppDevice>(); }}

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
        runEvery(
            "fetchFast", [plc] { plc->fetchFast(); }, 1);
        if (plc->getDeviceType() != DeviceUnion::DevCircle)
        {
            runEvery(
                "fetchTest", [plc] { plc->fetchTest(); }, 500);
        }
        runEvery(
            "fetchLazy", [plc] { plc->fetchLazy(); }, 1000);
        runEvery(
            "PLCHeart", [plc] { plc->heart(); }, 3000);
        runEvery(
            "PLCKeepConnection", [plc] { plc->keepConnection(); }, 1000);
        QObject::connect(plc, &BasePLCDevice::signalState,
                         [this](const uint32_t status) { AppPlcExtension::afterPLCStatesChange(status); });
        QObject::connect(plc, &BasePLCDevice::signalQR, [this](const uint32_t count) {
            AppStatisticsExtension::afterTriggerChanged(AppPLCCountType::code_trigger, count);
        });
        QObject::connect(plc, &BasePLCDevice::signalOCR, [this](const uint32_t count) {
            AppStatisticsExtension::afterTriggerChanged(AppPLCCountType::ocr_trigger, count);
        });
        QObject::connect(plc, &BasePLCDevice::signalLocate,
                         [this]() { AppStatisticsExtension::afterTriggerChanged(AppPLCCountType::locate_trigger); });
        QObject::connect(plc, &BasePLCDevice::signalCheck,
                         [this]() { AppStatisticsExtension::afterTriggerChanged(AppPLCCountType::check_trigger); });

        QObject::connect(plc, &BasePLCDevice::signalIn, [this](const uint32_t count) {
            AppProductionExtension::afterProductIn(count);
            AppStatisticsExtension::afterTriggerChanged(AppPLCCountType::amount, count);
            LogInfo("amount change: {}", count);
        });
        QObject::connect(plc, &BasePLCDevice::signalOut,
                         [this](const uint32_t count) { AppProductionExtension::afterProductOut(count); });
        if (strType == "boxApp")
        {
            QObject::connect(plc, &BasePLCDevice::triggerImage, [this]() { AppQmlExtension::collectTest(); });
        }
    }
}

void AppFrame::AppFrameworkImpl::initCameraManager()
{
    if (config_["baumer"])
    {
        registerPlugin<CameraManager>(std::make_unique<CameraManager>());
        const auto &cameraManager = getPlugin<CameraManager>();
        cameraManager->init(config_);
        runEvery(
            "searchCamera", [cameraManager] { cameraManager->searchCamera(); }, 5000);
        runEvery(
            "captureImage", [cameraManager] { cameraManager->captureImageStart(); }, 10);
        QObject::connect(cameraManager, &CameraManager::signalImgGet,
                         [](const std::string title, const uint32_t count, cv::Mat mat) {
                             AppProductionExtension::afterGetImage(title, count, std::move(mat));
                         });
    }
}

void AppFrame::AppFrameworkImpl::initProduct()
{
    std::string strType = config_["product"]["type"].as<std::string>();
    using ProductRegistrationFunction = std::function<void()>;
    const std::unordered_map<std::string, ProductRegistrationFunction> productRegistrationMap = {
        {"circle", [this]() { registerAndInitProduct<CircleProduct>(config_); }},
        {"line", [this]() { registerAndInitProduct<LineProduct>(config_); }},
        {"cap", [this]() { registerAndInitProduct<CapProduct>(config_); }},
        {"impurity", [this]() { registerAndInitProduct<ImpurityProduct>(config_); }},
        {"boxApp", [this]() { registerAndInitProduct<BoxAppProduct>(config_); }},
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

void AppFrame::AppFrameworkImpl::beforClose()
{
    const auto &statistics = getPlugin<BaseStatistics>();
    statistics->sendReportEmit();
}

void AppFrame::AppFrameworkImpl::engineLoad(QQmlApplicationEngine *engine)
{
    std::string strType = config_["product"]["type"].as<std::string>();
    // 加载qml文件
    // Define a mapping for product type to title name and module name
    const std::unordered_map<std::string, std::pair<QString, QString>> productTypeMap = {
        {"line", {"尖庄100毫升光瓶瓶盖数据采集激光打码检测设备", "MainWindow"}},
        {"circle", {"尖庄500毫升光瓶瓶盖数据采集激光打码检测设备", "MainWindow"}},
        {"cap", {"尖庄100毫升光瓶瓶盖在线检测设备", "MainWindow"}},
        {"impurity", {"酒内异物检测设备", "MainWindow"}},
        {"boxApp", {"酒盒外观缺陷检测设备", "MainWindow"}}
        // Add more mappings as needed
    };

    auto it = productTypeMap.find(strType);
    if (it != productTypeMap.end())
    {
        const auto &[titleName, moduleName] = it->second;

        // 注册MyObject类
        qmlRegisterType<ImagePainter>("AppFrame", 1, 0, "ImagePainter");
        engine->rootContext()->setContextProperty("appMetaFlash", getMeta());
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