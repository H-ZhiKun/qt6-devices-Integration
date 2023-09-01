#include "AppFrameworkImpl.h"
#include "AlertWapper.h"
#include "AppFramework.h"
#include "AppMetaFlash.h"
#include "DBConnectionPool.h"
#include "Domino.h"
#include "FormulaWapper.h"
#include "MysqlConnectionPool.h"
#include "UserWapper.h"
#include "Utils.h"
#include <QBuffer>
#include <QCoreApplication>
#include <QDebug>
#include <QImage>
#include <QJsonArray>
#include <QJsonObject>
#include <QObject>
#include <QThread>
using namespace AppFrame;

AppFramework &AppFramework::instance()
{
    return AppFrameworkImpl::instance();
}

AppFrame::AppFrameworkImpl::AppFrameworkImpl()
{
    QObject::connect(qApp, &QCoreApplication::aboutToQuit, [this]() { memoryClean(); });
    registerExpectation(ExpectedFunction::DominoConnect,
                        std::bind(&AppFrameworkImpl::dominoConnect, this, std::placeholders::_1));
    registerExpectation(ExpectedFunction::DeleteFormula,
                        std::bind(&AppFrameworkImpl::deleteFormula, this, std::placeholders::_1));
    registerExpectation(ExpectedFunction::SelectFormula,
                        std::bind(&AppFrameworkImpl::selectFormula, this, std::placeholders::_1));
    registerExpectation(ExpectedFunction::ModifyFormula,
                        std::bind(&AppFrameworkImpl::modifyFormula, this, std::placeholders::_1));
    registerExpectation(ExpectedFunction::InsertFormula,
                        std::bind(&AppFrameworkImpl::insertFormula, this, std::placeholders::_1));
    registerExpectation(ExpectedFunction::GetCameraParam,
                        std::bind(&AppFrameworkImpl::getCameraParam, this, std::placeholders::_1));
    registerExpectation(ExpectedFunction::SetCameraParam,
                        std::bind(&AppFrameworkImpl::setCameraParam, this, std::placeholders::_1));
    registerExpectation(ExpectedFunction::InsertUser,
                        std::bind(&AppFrameworkImpl::insertUser, this, std::placeholders::_1));
    registerExpectation(ExpectedFunction::SelectUserID,
                        std::bind(&AppFrameworkImpl::selectUserID, this, std::placeholders::_1));
    registerExpectation(ExpectedFunction::SelectUser,
                        std::bind(&AppFrameworkImpl::selectUser, this, std::placeholders::_1));
    registerExpectation(ExpectedFunction::DeleteUser,
                        std::bind(&AppFrameworkImpl::deleteUser, this, std::placeholders::_1));
    registerExpectation(ExpectedFunction::ModifyUser,
                        std::bind(&AppFrameworkImpl::modifyUser, this, std::placeholders::_1));
    registerExpectation(ExpectedFunction::SelectAlert,
                        std::bind(&AppFrameworkImpl::selectAlert, this, std::placeholders::_1));
    registerExpectation(ExpectedFunction::CollectImage,
                        std::bind(&AppFrameworkImpl::collectImage, this, std::placeholders::_1));
    registerExpectation(ExpectedFunction::ReadPLC, std::bind(&AppFrameworkImpl::readPLC, this, std::placeholders::_1));
    registerExpectation(ExpectedFunction::WritePLC,
                        std::bind(&AppFrameworkImpl::writePLC, this, std::placeholders::_1));
}

AppFrame::AppFrameworkImpl::~AppFrameworkImpl() noexcept
{
}

int AppFrame::AppFrameworkImpl::run()
{
    // 加载配置文件
    loadConfig();
    initLogger();
    initFile();
    initSqlHelper();
    initNetworkClient();
    initBaumerManager();
    initPLC();
    timerTask();
    return 0;
}

std::string AppFrame::AppFrameworkImpl::expected(const ExpectedFunction &expectedType, const std::string &jsValue)
{
    std::string strRet = Utils::makeResponse(false, "function not find");
    if (mapExpectedFunction_.find(expectedType) != mapExpectedFunction_.end())
    {
        strRet = mapExpectedFunction_[expectedType](jsValue);
    }
    return strRet;
}

bool AppFrame::AppFrameworkImpl::registerExpectation(const ExpectedFunction &expectedType,
                                                     std::function<std::string(const std::string &)> &&api)
{
    bool ret = false;
    if (mapExpectedFunction_.find(expectedType) == mapExpectedFunction_.end())
    {
        mapExpectedFunction_[expectedType] = api;
        ret = true;
    }
    return ret;
}

void AppFrame::AppFrameworkImpl::storeImagePainter(const DisplayWindows &painterId, void *obj)
{
    if (obj != nullptr)
    {
        mapStorePainter_[painterId] = static_cast<QObject *>(obj);
    }
}

std::string AppFrame::AppFrameworkImpl::dominoConnect(const std::string &value)
{
    bool ret = false;
    Json::Value jsTemp = Utils::stringToJson(value);
    std::string ip = jsTemp["ip"].asString();
    uint16_t port = jsTemp["port"].asUInt();
    if (!ip.empty() && port > 0)
        ret = invokeCpp(domino_, domino_->invokeConnectToServer, ip, port);
    return Utils::makeResponse(ret);
}

std::string AppFrame::AppFrameworkImpl::deleteFormula(const std::string &value)
{
    bool ret = false;
    Json::Value jsTemp = Utils::stringToJson(value);
    std::string name = jsTemp["name"].asString();
    if (!name.empty())
        ret = FormulaWapper::deleteFormula(name.c_str());
    return Utils::makeResponse(ret);
}

std::string AppFrame::AppFrameworkImpl::modifyFormula(const std::string &value)
{
    bool ret = false;
    ret = FormulaWapper::modifyFormula(value.c_str());
    return Utils::makeResponse(ret);
}

std::string AppFrame::AppFrameworkImpl::insertFormula(const std::string &value)
{
    bool ret = false;
    ret = FormulaWapper::insertFormula(value.c_str());
    return Utils::makeResponse(ret);
}

std::string AppFrame::AppFrameworkImpl::selectFormula(const std::string &value)
{
    bool ret = false;
    Json::Value jsTemp = Utils::stringToJson(value);
    std::string name = jsTemp["name"].asString();
    QString jsRet;
    if (!name.empty())
    {
        jsRet = FormulaWapper::selectOneFormula(name.c_str());
        if (!jsRet.isEmpty())
        {
            ret = true;
        }
    }
    return Utils::makeResponse(ret, std::move(Utils::stringToJson(jsRet.toStdString())));
}

void AppFrame::AppFrameworkImpl::updateUserData()
{
    Json::Value jsUserlaVal = UserWapper::selectAllUser();
    for (const Json::Value &jsonSingleValue : jsUserlaVal)
    {
        invokeCpp(&AppMetaFlash::instance(), AppMetaFlash::instance().invokeRuntimeRoutine,
                  Q_ARG(PageIndex, PageIndex::PageUser), Q_ARG(QString, Utils::jsonToString(jsonSingleValue).c_str()));
    }
}

std::string AppFrame::AppFrameworkImpl::getCameraParam(const std::string &value)
{
    bool ret = false;
    auto params = Utils::stringToJson(value);
    uint8_t winId = params["display_window"].asInt();
    Json::Value jsVal = baumerManager_->getCamera(winId);
    std::string des;
    if (jsVal.isNull())
    {
        des = "camera init failed";
    }
    else
    {
        ret = true;
    }
    return Utils::makeResponse(ret, std::move(jsVal), std::move(des));
}

std::string AppFrame::AppFrameworkImpl::setCameraParam(const std::string &value)
{
    bool ret = false;
    Json::Value jsParams = Utils::stringToJson(value);
    std::string des;
    if (!jsParams.isNull())
    {
        ret = baumerManager_->setCamera(jsParams, des);
    }
    return Utils::makeResponse(ret, {}, std::move(des));
}

std::string AppFrame::AppFrameworkImpl::insertUser(const std::string &value)
{
    bool ret = false;
    ret = UserWapper::insertUser(value.c_str());
    return Utils::makeResponse(ret);
}

std::string AppFrame::AppFrameworkImpl::selectUserID(const std::string &value)
{
    bool ret = false;
    Json::Value jsParams;
    std::string jsRet = UserWapper::selectUserID(value.c_str()).toStdString();
    if (!jsRet.empty())
    {
        ret = true;
        jsParams["id"] = jsRet;
    }
    return Utils::makeResponse(ret, std::move(jsParams));
}

std::string AppFrame::AppFrameworkImpl::selectUser(const std::string &value)
{
    bool ret = false;
    Json::Value jsTemp = Utils::stringToJson(value);
    std::string name = jsTemp["name"].asString();
    QString jsRet;
    if (!name.empty())
    {
        jsRet = UserWapper::selectUser(name.c_str());
        if (!jsRet.isEmpty())
        {
            ret = true;
        }
    }
    return Utils::makeResponse(ret, std::move(Utils::stringToJson(jsRet.toStdString())));
}

std::string AppFrame::AppFrameworkImpl::deleteUser(const std::string &value)
{
    bool ret = false;
    Json::Value jsParams = Utils::stringToJson(value);
    std::string id = jsParams["id"].asString();
    if (!id.empty())
    {
        ret = UserWapper::deleteUser(id.c_str());
    }
    return Utils::makeResponse(ret);
}

std::string AppFrame::AppFrameworkImpl::modifyUser(const std::string &value)
{
    bool ret = false;
    ret = UserWapper::modifyUser(value.c_str());
    return Utils::makeResponse(ret);
}

std::string AppFrame::AppFrameworkImpl::selectAlert(const std::string &value)
{
    bool ret = false;
    Json::Value jsParams = Utils::stringToJson(value);
    Json::Value jsAlertVal =
        AlertWapper::selectAlertDataPaged(jsParams["pageSize"].asInt(), jsParams["pageNumber"].asInt(), "", "id desc");
    int num = AlertWapper::alertNum();
    if (!jsAlertVal.empty() && num != 0)
    {
        ret = true;
    }
    Json::Value jsRet;
    jsRet["num"] = num;
    for (Json::Value &jsonSingleValue : jsAlertVal)
    {
        std::string datetimeStr = jsonSingleValue["created_time"].asString();
        // 替换T为空格
        size_t pos = datetimeStr.find('T');
        if (pos != std::string::npos)
        {
            datetimeStr.replace(pos, 1, " ");
        }

        // 去除末尾的毫秒部分
        pos = datetimeStr.find('.');
        if (pos != std::string::npos)
        {
            datetimeStr.erase(pos);
        }
        jsonSingleValue["created_time"] = datetimeStr;
        if (!jsonSingleValue["updated_time"].empty())
        {
            std::string datetimeStrUp = jsonSingleValue["updated_time"].asString();
            // 替换T为空格
            size_t posUp = datetimeStrUp.find('T');
            if (posUp != std::string::npos)
            {
                datetimeStrUp.replace(posUp, 1, " ");
            }

            // 去除末尾的毫秒部分
            posUp = datetimeStrUp.find('.');
            if (posUp != std::string::npos)
            {
                datetimeStrUp.erase(posUp);
            }
            jsonSingleValue["updated_time"] = datetimeStrUp;
        }

        invokeCpp(&AppMetaFlash::instance(), AppMetaFlash::instance().invokeRuntimeRoutine,
                  Q_ARG(PageIndex, PageIndex::PageAlarm), Q_ARG(QString, Utils::jsonToString(jsonSingleValue).c_str()));
    }
    return Utils::makeResponse(ret, std::move(jsRet));
}

std::string AppFrame::AppFrameworkImpl::collectImage(const std::string &)
{
    bool ret = false;
    saveImageFlag.store(true, std::memory_order_release);
    ret = true;
    return Utils::makeResponse(ret);
}

std::string AppFrame::AppFrameworkImpl::readPLC(const std::string &value)
{
    bool ret = false;
    Json::Value jsParams = Utils::stringToJson(value);
    Json::Value res;
    for (const auto &key : jsParams.getMemberNames())
    {
        std::string temp;
        auto vKeys = Utils::splitString(key, "_");
        if (vKeys.size() == 3)
        {
            temp = plcDev_->readDevice(vKeys[1], vKeys[2]);
            qDebug() << "temp" << temp;
        }
        else if (vKeys.size() == 4)
        {
            temp = plcDev_->readDevice(vKeys[1], vKeys[2], vKeys[3]);
        }
        else
        {
            continue;
        }
        if (!temp.empty())
        {
            res[key] = temp;
            ret = true;
        }
    }
    std::string result = Utils::makeResponse(ret, std::move(res));
    qDebug() << result;
    return result;
}

std::string AppFrame::AppFrameworkImpl::writePLC(const std::string &value)
{
    // 例：key:M071_b_12288_14  count=4   value  "1"或"0"
    // 例：key:M071_r_12288  count=3   value    "0.76"
    bool ret = false;
    Json::Value jsParams = Utils::stringToJson(value);
    for (const auto &key : jsParams.getMemberNames())
    {
        std::string temp;
        const std::string &curValue = jsParams[key].asString();
        auto vKeys = Utils::splitString(key, "_");
        if (vKeys.size() == 3)
        {
            ret = plcDev_->writeDataToDevice(vKeys[1], vKeys[2], "0", curValue);
            if (ret == false)
            {
                LogWarn("write data faile! address: {}", value);
            }
        }
        else if (vKeys.size() == 4)
        {
            ret = plcDev_->writeDataToDevice(vKeys[1], vKeys[2], vKeys[3], curValue);
            if (ret == false)
            {
                LogWarn("write data faile! address: {}", value);
            }
        }
        else
        {
            continue;
        }
    }
    return Utils::makeResponse(ret);
}

void AppFrame::AppFrameworkImpl::loadConfig()
{
    strAppPath_ = qApp->applicationDirPath().toStdString();
    try
    {
        std::string filePath = qApp->applicationDirPath().toStdString() + "/config.yaml";
        config_ = std::move(YAML::LoadFile(filePath));
        LogInfo("load config yaml success.");
    }
    catch (const YAML::Exception &e)
    {
        LogError("Error parsing YAML: {}", e.what());
        Utils::appExit(-1);
    }
}

void AppFrame::AppFrameworkImpl::saveConfig()
{
    try
    {
        baumerManager_->saveConfig(config_);
        std::string filePath = strAppPath_ + "/config.yaml";
        // 保存修改后的配置回文件
        std::ofstream fout(filePath);
        fout << config_;
        fout.close();
        LogInfo("saveConfig success.");
    }
    catch (const YAML::Exception &e)
    {
        LogError("Error save YAML: {}", e.what());
    }
}

void AppFrame::AppFrameworkImpl::initLogger()
{
    // 初始化日志记录器
    std::string logPath =
        strAppPath_ + std::string("/logs/") + config_["app"]["log"]["log_file_name"].as<std::string>();
    size_t logLevel = config_["app"]["log"]["log_level"].as<size_t>();
    size_t logFileCount = config_["app"]["log"]["log_file_count"].as<size_t>();
    size_t logFileSize = Utils::anyFromString<size_t>(config_["app"]["log"]["log_file_size"].as<std::string>());
    CLogger::GetLogger().initLogger(logPath, logLevel, logFileSize, logFileCount);
    LogInfo("AppFrame Run");
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
        memoryClean();
        Utils::appExit(-1);
    }
    LogInfo("sqlhelper init success.");
    updateFormulaData(); // 放在异步处理中会丢失
    updateUserData();
}

void AppFrame::AppFrameworkImpl::initNetworkClient()
{
    std::string dominoIp = config_["domino"]["host"].as<std::string>();
    uint16_t dominoPort = config_["domino"]["port"].as<uint16_t>();
    std::string cognexIp = config_["cognex"]["host"].as<std::string>();
    uint16_t cognexPort = config_["cognex"]["port"].as<uint16_t>();
    std::string permissionIp = config_["permission"]["host"].as<std::string>();
    uint16_t permissionPort = config_["permission"]["port"].as<uint16_t>();
    domino_ = new Domino();
    domino_->startClient(dominoIp.c_str(), dominoPort);
    cognex_ = new Cognex();
    cognex_->startClient(cognexIp.c_str(), cognexPort);
    permission_ = new Permission();
    permission_->startClient(permissionIp.c_str(), permissionPort);
    webManager_ = new WebManager();
    webManager_->init(config_);
    QObject::connect(webManager_, &WebManager::ocrRecv, [this](const std::string &json) { processPaddleOCR(json); });
    QObject::connect(webManager_, &WebManager::tangleRecv,
                     [this](const std::string &json) { processYoloTangle(json); });
    QObject::connect(webManager_, &WebManager::tangleCheckRecv,
                     [this](const std::string &json) { processYoloTangle(json); });
    LogInfo("network client start success.");
    // 获取到二维码并发送
    QObject::connect(cognex_, &Cognex::finishReadQRCode, [this](const std::string value) { processQrCode(value); });
    // 获取到物流码并存储
    QObject::connect(permission_, &Permission::codeRight,
                     [this](const std::string code1, const std::string code2) { processCode(code1, code2); });
}

void AppFrame::AppFrameworkImpl::initPLC()
{
    std::string plcType = config_["plc"]["type"].as<std::string>();
    std::string plcIp = config_["plc"]["host"].as<std::string>();
    uint16_t plcPort = config_["plc"]["port"].as<uint16_t>();
    uint16_t io = config_["plc"]["io_freq"].as<uint16_t>();
    uint16_t fifo = config_["plc"]["fifo_freq"].as<uint16_t>();
    plcDev_ = new PLCDevice;
    plcDev_->init(plcIp, plcPort, io, fifo);
    LogInfo("{} PLC device start success.", plcType);
    QObject::connect(plcDev_, &PLCDevice::locatePhoto, [this](const uint64_t bottomNum) { refreshLocate(bottomNum); });
    QObject::connect(plcDev_, &PLCDevice::locateCheckPhoto,
                     [this](const uint64_t bottomNum) { refreshLocateCheck(bottomNum); });
    QObject::connect(plcDev_, &PLCDevice::codeCheck, [this](const uint64_t bottomNum) { refreshCodeCheck(bottomNum); });

    // 获得读二维码信号
    QObject::connect(plcDev_, &PLCDevice::readQRCode, [this](uint8_t bottomNum) {
        cognex_->scanOnce();
        productList_.push_back(new Product());
    });

    QObject::connect(plcDev_, &PLCDevice::codeLogistics, [this](uint8_t bottomNum) { doPrintCode(bottomNum); });
}

void AppFrame::AppFrameworkImpl::updateRealData()
{

    Json::Value jsMainVal;
    jsMainVal["count_all"] = 4;             // 进瓶数
    jsMainVal["count_pass"] = 22590;        // 合格品数
    jsMainVal["count_waste"] = 7;           // 废品总数
    jsMainVal["count_locate_waste"] = 3;    // 定位废品数
    jsMainVal["count_code_waste"] = 4;      // 喷码废品数
    jsMainVal["count_pause_waste"] = 0;     // 暂停、终止废品数
    jsMainVal["equipmentSteps"] = "未启动"; // 设备步骤
    jsMainVal["produceState"] = 3;          // 生产状态
    jsMainVal["dominoState"] = std::to_string(domino_->getConnect());
    jsMainVal["cognexState"] = std::to_string(cognex_->getConnect());
    jsMainVal["permissionState"] = std::to_string(permission_->getConnect());
    jsMainVal["plcState"] = std::to_string(plcDev_->getConnect());
    invokeCpp(&AppMetaFlash::instance(), AppMetaFlash::instance().invokeRuntimeRoutine,
              Q_ARG(PageIndex, PageIndex::PageMain), Q_ARG(QString, Utils::jsonToString(jsMainVal).c_str()));
}

void AppFrame::AppFrameworkImpl::updateProduceRealData()
{
    Json::Value jsProduceVal;
    /*生产数据界面实时更新数据*/
    jsProduceVal["positive_active_energy"] = plcDev_->readDevice("r", "12586"); // 正向有功电能
    jsProduceVal["reverse_active_energy"] = plcDev_->readDevice("r", "12588");  // 反向有功电能
    jsProduceVal["a_phase_voltage"] = plcDev_->readDevice("r", "12590");        // A相电压
    jsProduceVal["b_phase_voltage"] = plcDev_->readDevice("r", "12592");        // B相电压
    jsProduceVal["c_phase_voltage"] = plcDev_->readDevice("r", "12594");        // C相电压
    jsProduceVal["temperature"] = plcDev_->readDevice("r", "12608");            // 温度
    jsProduceVal["total_active_power"] = plcDev_->readDevice("r", "12602");     // 总有功功率
    jsProduceVal["total_apparent_power"] = plcDev_->readDevice("r", "12604");   // 总视在功率
    jsProduceVal["total_active_energy"] = plcDev_->readDevice("r", "12606");    // 总有功电能
    jsProduceVal["a_direction_current"] = plcDev_->readDevice("r", "12596");    // A向电流
    jsProduceVal["b_direction_current"] = plcDev_->readDevice("r", "12598");    // B向电流
    jsProduceVal["c_direction_current"] = plcDev_->readDevice("r", "12600");    // C向电流
    jsProduceVal["humidity"] = plcDev_->readDevice("r", "12610");               // 湿度

    invokeCpp(&AppMetaFlash::instance(), AppMetaFlash::instance().invokeRuntimeRoutine,
              Q_ARG(PageIndex, PageIndex::PageProduce), Q_ARG(QString, Utils::jsonToString(jsProduceVal).c_str()));
}

void AppFrame::AppFrameworkImpl::updateAlertData()
{
    Json::Value jsAlertVal = AlertWapper::selectAlertDataPaged(7, 1);
    for (Json::Value &jsonSingleValue : jsAlertVal)
    {
        invokeCpp(&AppMetaFlash::instance(), AppMetaFlash::instance().invokeRuntimeRoutine,
                  Q_ARG(PageIndex, PageIndex::PageAlarm), Q_ARG(QString, Utils::jsonToString(jsonSingleValue).c_str()));
    }
}

void AppFrame::AppFrameworkImpl::updateFormulaData()
{
    Json::Value jsFormulaVal = FormulaWapper::selectAllFormula();
    for (const Json::Value &jsonSingleValue : jsFormulaVal)
    {
        invokeCpp(&AppMetaFlash::instance(), AppMetaFlash::instance().invokeRuntimeRoutine,
                  Q_ARG(PageIndex, PageIndex::PageFormula),
                  Q_ARG(QString, Utils::jsonToString(jsonSingleValue).c_str()));
    }
}

void AppFrame::AppFrameworkImpl::updateVideo()
{
    // std::shared_lock lock(mtxSNPainter_);
    // for (const auto &[key, value] : mapStorePainter_)
    // {
    //     auto windId = key;
    //     uint8_t cameraId = static_cast<uint8_t>(windId);
    //     std::list<cv::Mat> matData = baumerManager_->getImageBySN(cameraId);
    //     if (matData.size() == 0)
    //     {
    //         continue;
    //     }
    //     cv::Mat temp = matData.back();
    //     Utils::asyncTask([this, windId, temp] {
    //         QImage img = Utils::matToQImage(temp);
    //         if (img.isNull() == false)
    //         {
    //             if (saveImageFlag.load(std::memory_order_acquire))
    //             {
    //                 saveImageFlag.store(false, std::memory_order_release);
    //                 saveImageToFile(img, windId);
    //             }
    //             saveImageToFile(img, windId);

    //             invokeCpp(mapStorePainter_[windId], "updateImage", Q_ARG(QImage, img));
    //             if (windId == DisplayWindows::LocationCamera)
    //             {
    //                 LogInfo("send locateImage");
    //                 productList_.back()->locateImageName = Utils::getCurrentTime(true);
    //                 productList_.back()->locateImage = temp;
    //                 std::string url = config_["algorithm"]["url_predict"].as<std::string>();
    //                 invokeCpp(httpClient_, "sendPostRequest", Q_ARG(std::string, url),
    //                           Q_ARG(std::string, Utils::makeHttpBodyWithCVMat(
    //                                                  temp, 6, productList_.back()->locateImageName, "tangle")));
    //             }
    //         }
    //     });
    // }
}

void AppFrame::AppFrameworkImpl::refreshCodeCheck(const uint64_t bottomNum)
{
    std::list<cv::Mat> matData = baumerManager_->getImageBySN(1);
    LogInfo("Get image size = {}, window = 1", matData.size());
    if (matData.size() == 0)
    {
        qDebug() << fmt::format("refreshImage mat null wind = 1, bottomNum = {}", bottomNum);
        return;
    }
    cv::Mat temp = matData.back();
    Utils::asyncTask([this, temp, bottomNum] {
        std::string url;
        std::string imageName = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss_zzz").toStdString() + "CC";
        static uint16_t countCode = 0;
        url = config_["algorithm"]["url_ocr"].as<std::string>();
        LogInfo("CodeCheckCamera count: {}", countCode);
        if (productList_.size() == 0)
        {
            LogInfo("add product in code check func");
            productList_.push_back(new Product());
        }
        for (auto &product_ : productList_)
        {
            if (product_->codeCheckImage.empty() && !product_->logistics1.empty())
            {
                product_->codeCheckImage = temp.clone();
                product_->codeCheckImageName = imageName;
                // invokeCpp(httpClient_, "sendPostRequest", Q_ARG(std::string, url),
                //           Q_ARG(std::string, Utils::makeHttpBodyWithCVMat(temp, bottomNum, imageName, "paddleOCR")));
                break;
            }
        }
        countCode++;
        QImage img = Utils::matToQImage(temp);
        invokeCpp(mapStorePainter_[DisplayWindows::CodeCheckCamera], "updateImage", Q_ARG(QImage, img));
    });
}

void AppFrame::AppFrameworkImpl::refreshLocateCheck(const uint64_t bottomNum)
{
    std::list<cv::Mat> matData = baumerManager_->getImageBySN(2);
    LogInfo("Get image size = {}, window = 2", matData.size());
    if (matData.size() == 0)
    {
        qDebug() << fmt::format("refreshImage mat null wind = 2, bottomNum = {}", bottomNum);
        return;
    }
    cv::Mat temp = matData.back();
    Utils::asyncTask([this, temp, bottomNum] {
        std::string url;
        std::string imageName = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss_zzz").toStdString() + "LC";
        static uint16_t countLocateCheck = 0;
        url = config_["algorithm"]["url_predict"].as<std::string>();
        LogInfo("LocateCheckCamera: {}", countLocateCheck);
        countLocateCheck++;
        if (productList_.size() == 0)
        {
            LogInfo("add product in locate check func");
            productList_.push_back(new Product());
        }
        for (auto &product_ : productList_)
        {
            if (product_->locateCheckImage.empty())
            {
                LogInfo("send locate check image to algorithm");
                product_->locateCheckImage = temp.clone();
                product_->locateCheckImageName = imageName;
                std::string jsonData;
                QByteArray byteArray;
                Utils::makeJsonAndByteArray(temp, bottomNum, imageName, "tangleCheck", strTangleCheckPath_, jsonData,
                                            byteArray);
                invokeCpp(webManager_, "sendToALGO", Q_ARG(uint8_t, 2), Q_ARG(std::string, jsonData),
                          Q_ARG(QByteArray, byteArray));
                break;
            }
        }
        QImage img = Utils::matToQImage(temp);
        invokeCpp(mapStorePainter_[DisplayWindows::LocateCheckCamera], "updateImage", Q_ARG(QImage, img));
    });
}

void AppFrame::AppFrameworkImpl::refreshLocate(const uint64_t bottomNum)
{
    Utils::asyncTask([this, bottomNum] {
        LogInfo("tangle timer: get singnal, bottom {}", bottomNum);
        std::list<cv::Mat> matData = baumerManager_->getImageBySN(0);
        auto startTime = std::chrono::steady_clock::now(); // 记录开始时间
        while (true)
        {
            // 执行您的循环操作
            matData = baumerManager_->getImageBySN(0);
            auto currentTime = std::chrono::steady_clock::now(); // 获取当前时间
            auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime);

            if (elapsedTime.count() >= 2500 || matData.size())
            {
                // 如果经过200毫秒或更长时间，退出循环
                break;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        if (matData.size() == 0)
        {
            qDebug() << fmt::format("refreshImage mat timeout, bottomNum: {}", bottomNum);
            return;
        }
        LogInfo("tangle timer: get image, size: {}", matData.size());
        cv::Mat temp = matData.back();
        cv::resize(temp, temp, {800, 800});
        LogInfo("mat resize cols: {}, rows: {}", temp.cols, temp.rows);

        std::string url;
        std::string imageName = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss_zzz").toStdString() + "L";
        static uint16_t countLocation = 0;
        url = config_["algorithm"]["url_predict"].as<std::string>();
        LogInfo("LocationCamera: {}", countLocation);
        LogInfo("productList_ cur size: {}", productList_.size());
        countLocation++;
        if (productList_.size() == 0)
        {
            LogInfo("add product in locate func");
            productList_.push_back(new Product());
        }
        for (auto &product_ : productList_)
        {
            if (product_->locateImage.empty())
            {
                LogInfo("tangle timer: send image to algothm");
                product_->locateImage = temp.clone();
                product_->locateImageName = imageName;
                std::string jsonData;
                QByteArray byteArray;
                Utils::makeJsonAndByteArray(temp, bottomNum, imageName, "tangle", strTanglePath_, jsonData, byteArray);
                invokeCpp(webManager_, "sendToALGO", Q_ARG(uint8_t, 0), Q_ARG(std::string, jsonData),
                          Q_ARG(QByteArray, byteArray));
                break;
            }
        }
        QImage img = Utils::matToQImage(temp);
        invokeCpp(mapStorePainter_[DisplayWindows::LocationCamera], "updateImage", Q_ARG(QImage, img));
    });
}
void AppFrame::AppFrameworkImpl::refreshImageTest(const int bottomNum)
{
    // cv::Mat temp(400, 400, CV_8UC3, cv::Scalar(255, 255, 255));
    // Utils::asyncTask([this, target = std::move(temp), bottomNum] {
    //     std::string url;
    //     std::string imageName = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss_zzz").toStdString();
    //     std::string modelName = "tangle";
    //     url = config_["algorithm"]["url_predict"].as<std::string>();
    //     LogInfo("LocationCamera bottom: ", bottomNum);
    //     invokeCpp(httpClient_, "sendPostRequest", Q_ARG(std::string, url),
    //               Q_ARG(std::string, Utils::makeHttpBodyWithCVMat(target, bottomNum, imageName, modelName)));
    //     QImage img = Utils::matToQImage(target);
    //     if (img.isNull() == false)
    //     {
    //         invokeCpp(mapStorePainter_[DisplayWindows::LocationCamera], "updateImage", Q_ARG(QImage, img));
    //     }
    // });
}
void AppFrame::AppFrameworkImpl::updateByMinute(const std::string &minute)
{
    // 每分钟更新电能数据
    QList<QVariantMap> dataList;
    Json::Value dataEle;
    // 与PLC通信得到数据
    float num = atoi(minute.c_str());
    // dataEle["id"] = 0;
    dataEle["positive_active_energy"] = 34.54;
    dataEle["reverse_active_energy"] = 43.5452;
    dataEle["a_phase_voltage"] = num;
    dataEle["b_phase_voltage"] = 43.45;
    dataEle["c_phase_voltage"] = 43.475;
    dataEle["temperature"] = 43.0;
    dataEle["total_active_power"] = 43.045;
    dataEle["total_apparent_power"] = 43.00;
    dataEle["total_active_energy"] = 43.0745;

    dataEle["a_direction_current"] = 43.454545;
    dataEle["b_direction_current"] = 43.45;
    dataEle["c_direction_current"] = 43.77;
    dataEle["humidity"] = 43;

    PgsqlHelper::getSqlHelper().insertData("electric_data", std::move(dataEle));
    // updateAlarmData("test"); // TODO：有报错才发送
}

void AppFrame::AppFrameworkImpl::updateByDay(const std::string &year, const std::string &month, const std::string &day)
{
    // 每日创建当月份数据表和下月份数据表做冗余
    // 动态创建月份数据库表
    std::string monthSingleBoottleTB = year + month + "single_bottle";
    std::list<std::string> fields{"id SERIAL PRIMARY KEY",
                                  "qr_code_result varchar(256)",
                                  "logistics_code_gt char(24)",
                                  "locate_camera_image varchar(256)",
                                  "locate_res real",
                                  "locate_check_camera_image varchar(256)",
                                  "locate_check_res boolean",
                                  "code_check_camera_image varchar(256)",
                                  "logistics_code char(24)",
                                  "logistics_code_res boolean",
                                  "batch_num varchar(256)",
                                  "formula_name varchar(128)",
                                  "created_time timestamp DEFAULT CURRENT_TIMESTAMP",
                                  "UNIQUE (id)"};

    if (PgsqlHelper::getSqlHelper().createTable(monthSingleBoottleTB, std::move(fields)))
    {
        LogInfo("This month table created successfully");
    }
    else
    {
        LogInfo("Failed to create this month table");
    }

    int tempMonth = atoi(month.c_str());
    std::string lastMonth;
    // 月份数字小于10，前面需要加上0
    if (++tempMonth < 10)
    {
        lastMonth = "0" + std::to_string(tempMonth);
    }

    // 动态创建下月份数据库表
    std::string lastMonthSingle = year + lastMonth + "single_bottle";
    std::list<std::string> lastFields{"id SERIAL PRIMARY KEY",
                                      "qr_code_reslut varchar(256)",
                                      "logistics_code_gt char(24)",
                                      "locate_camera_image varchar(256)",
                                      "locate_res real",
                                      "locate_check_camera_image varchar(256)",
                                      "locate_check_res boolean",
                                      "code_check_camera_image varchar(256)",
                                      "logistics_code char(24)",
                                      "logistics_code_res boolean",
                                      "batch_num varchar(256)",
                                      "formula_name varchar(128)",
                                      "created_time timestamp DEFAULT CURRENT_TIMESTAMP",
                                      "UNIQUE (id)"};
    if (PgsqlHelper::getSqlHelper().createTable(lastMonthSingle, std::move(lastFields)))
    {
        LogInfo("Last month table created successfully");
    }
    else
    {
        LogInfo("Failed to create Last month table");
    }
}

void AppFrame::AppFrameworkImpl::updateSensorRealData()
{
    Json::Value jsSensorVal;

    invokeCpp(&AppMetaFlash::instance(), AppMetaFlash::instance().invokeRuntimeRoutine,
              Q_ARG(PageIndex, PageIndex::PageSensor), Q_ARG(QString, Utils::jsonToString(jsSensorVal).c_str()));
}

void AppFrame::AppFrameworkImpl::updateValveRealData()
{
    Json::Value jsValveVal;

    invokeCpp(&AppMetaFlash::instance(), AppMetaFlash::instance().invokeRuntimeRoutine,
              Q_ARG(PageIndex, PageIndex::PageValve), Q_ARG(QString, Utils::jsonToString(jsValveVal).c_str()));
}

void AppFrame::AppFrameworkImpl::updatePowerRealData()
{
    Json::Value jsPowerVal;

    invokeCpp(&AppMetaFlash::instance(), AppMetaFlash::instance().invokeRuntimeRoutine,
              Q_ARG(PageIndex, PageIndex::PagePower), Q_ARG(QString, Utils::jsonToString(jsPowerVal).c_str()));
}

void AppFrame::AppFrameworkImpl::initBaumerManager()
{
    baumerManager_ = new BaumerManager();
    baumerManager_->start(config_);
}

void AppFrame::AppFrameworkImpl::initFile()
{
    QDir qdir;
    std::string saveImageDir = strAppPath_ + "/Image";
    if (!qdir.exists(saveImageDir.c_str()))
    {
        bool res = qdir.mkdir(saveImageDir.c_str());
        if (!res)
        {
            LogWarn("create saveImageDir dir file!");
        }
    }
    strTanglePath_ = saveImageDir + "/LocationCamera/";
    strOcrPath_ = saveImageDir + "/CodeCheckCamera/";
    strTangleCheckPath_ = saveImageDir + "/LocateCheckCamera/";
    if (!qdir.exists(strTanglePath_.c_str()))
    {
        bool res = qdir.mkdir(strTanglePath_.c_str());
        if (!res)
        {
            LogWarn("create LocationCamera dir file!");
        }
    }
    if (!qdir.exists(strOcrPath_.c_str()))
    {
        bool res = qdir.mkdir(strOcrPath_.c_str());
        if (!res)
        {
            LogWarn("create CodeCheckCamera dir file!");
        }
    }
    if (!qdir.exists(strTangleCheckPath_.c_str()))
    {
        bool res = qdir.mkdir(strTangleCheckPath_.c_str());
        if (!res)
        {
            LogWarn("create LocateCheckCamera dir file!");
        }
    }
}

// void AppFrame::AppFrameworkImpl::runHttp(const std::string &&modeleName, const std::string &imageName,
//                                          cv::Mat &matImage, const int bottomNum)
// {
//     std::string apiUrl;
//     if (matImage.empty())
//     {
//         LogWarn("Failed to read the image.");
//         return;
//     }
//     QImage saveImage = Utils::matToQImage(matImage);
//     // saveImageToFile(saveImage, DisplayWindows::CodeCheckCamera);
//     std::this_thread::sleep_for(std::chrono::milliseconds(50));
//     if (modeleName == "paddleOCR")
//     {
//         apiUrl = "http://192.168.101.8:5001/paddleOCR";
//         // 将图像转换为QByteArray
//         std::vector<uchar> buffer;
//         cv::imencode(".jpg", matImage, buffer);
//         QByteArray imageData(reinterpret_cast<const char *>(buffer.data()), buffer.size());

//         Json::Value jsVal;
//         jsVal["imageData"] = QString(imageData.toBase64()).toStdString(); // 将QByteArray转换为base64
//         jsVal["imageName"] = imageName;
//         jsVal["imageWidth"] = QString::number(matImage.cols).toStdString();
//         jsVal["imageHeight"] = QString::number(matImage.rows).toStdString();
//         std::string strJS = jsVal.toStyledString();
//         http_[0]->post(QString::fromStdString(apiUrl), QString::fromStdString(strJS), matImage, bottomNum);
//         std::string imagePath = "D:/deviceintegration/build/Debug/image/test.jpg";
//     }
//     else if (modeleName == "tangle" || modeleName == "tangleCheck")
//     {
//         // apiUrl = "http://192.168.101.8:5000/predict_tangle";
//         apiUrl = "http://127.0.0.1:5000/predict_tangle";
//         // 将图像转换为QByteArray
//         std::vector<uchar> buffer;
//         cv::imencode(".jpg", matImage, buffer);
//         QByteArray imageData(reinterpret_cast<const char *>(buffer.data()), buffer.size());

//         Json::Value jsVal;
//         jsVal["imageData"] = QString(imageData.toBase64()).toStdString(); // 将QByteArray转换为base64
//         jsVal["imageName"] = imageName;
//         jsVal["imageWidth"] = QString::number(matImage.cols).toStdString();
//         jsVal["imageHeight"] = QString::number(matImage.rows).toStdString();
//         std::string strJS = jsVal.toStyledString();
//         if (modeleName == "tangle")
//             http_[1]->post(QString::fromStdString(apiUrl), QString::fromStdString(strJS), matImage, bottomNum);
//         else if (modeleName == "tangleCheck")
//             http_[2]->post(QString::fromStdString(apiUrl), QString::fromStdString(strJS), matImage, bottomNum);
//     }
//     return;
// }

void AppFrame::AppFrameworkImpl::memoryClean()
{
    // 退出所有的子线程并回收线程栈资源，堆资源需要后续手动释放
    saveConfig();
    bThreadHolder = false;
    for (auto &ptr : lvFulltimeThread_)
    {
        ptr.join();
    }
    mapStorePainter_.clear();
    lvFulltimeThread_.clear();

    // 对象清理区域
    if (plcDev_ != nullptr)
    {
        delete plcDev_;
        plcDev_ = nullptr;
    }
    if (domino_ != nullptr)
    {
        delete domino_;
        domino_ = nullptr;
    }
    if (cognex_ != nullptr)
    {
        delete cognex_;
        cognex_ = nullptr;
    }
    if (permission_ != nullptr)
    {
        delete permission_;
        permission_ = nullptr;
    }
    if (webManager_ != nullptr)
    {
        delete webManager_;
        webManager_ = nullptr;
    }
    for (auto &ptr_ : productList_)
    {
        delete ptr_;
        ptr_ = nullptr;
    }
    if (baumerManager_ != nullptr)
    {
        delete baumerManager_;
        baumerManager_ = nullptr;
    }
}

void AppFrame::AppFrameworkImpl::timerTask()
{
    lvFulltimeThread_.push_back(std::thread([this] {
        std::string recYear = "-1", recMonth = "-1", recDay = "-1";
        std::string recHour = "-1", recMinute = "-1", recSecond = "-1";
        std::string year, month, day, hour, minute, second;
        while (bThreadHolder) // 线程退出Flag
        {
            // 实时更新数据
            updateRealData();
            updateProduceRealData();
            /*分解日期字符串*/
            Utils::getCurrentTime(year, month, day, hour, minute, second);
            if (recDay != day)
            { // 更新每日数据
                updateByDay(year, month, day);
                // 新增自然日变化逻辑
                std::string monthSingleBoottleTB = year + month + day;
                recDay = day;
            }

            if (recHour != hour)
            { // 更新每小时数据
            }

            if (recMinute != minute)
            { // 更新每分钟数据
                updateByMinute(minute);
                recMinute = minute;
            }
            if (second != recSecond)
            { // 更新每秒数据
                recSecond = second;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    }));

    // lvFulltimeThread_.push_back(std::thread([this] {
    //     // 视频渲染线程
    //     while (bThreadHolder)
    //     {
    //         updateVideo();
    //         std::this_thread::sleep_for(std::chrono::milliseconds(10));
    //     }
    // }));
}

// void AppFrame::AppFrameworkImpl::processHttpRes(const std::string &jsonData)
// {
//     // 调用yolo或者ocr处理过程
//     QString qString = QString::fromStdString(jsonData);
//     QJsonDocument jsonDocu = QJsonDocument::fromJson(qString.toUtf8());
//     QString type = jsonDocu["model"].toString();
//     if (type == "paddleOCR")
//     {
//         LogInfo("recieve paddleOCR algorithm return, in {}", Utils::getCurrentTime(true));
//         Utils::asyncTask([this, jsonDocu] { processPaddleOCR(jsonDocu); });
//     }
//     else if (type == "tangle")
//     {
//         LogInfo("recieve tangle algorithm return, in {}", Utils::getCurrentTime(true));
//         Utils::asyncTask([this, jsonDocu] { processYoloTangle(jsonDocu); });
//     }
// }

// void AppFrame::AppFrameworkImpl::processHttpResTest(const std::string &jsonData)
// {
//     QString qString = QString::fromStdString(jsonData);
//     QJsonDocument jsonDocu = QJsonDocument::fromJson(qString.toUtf8());
//     QString type = jsonDocu["model"].toString();
//     // cv::Mat test = cv::imread("D:/test.jpg");
//     cv::Mat test(400, 400, CV_8UC3, cv::Scalar(255, 255, 255));
//     Utils::asyncTask([this, jsonDocu, test] { processYoloTangleTest(jsonDocu, test); });
// }

void AppFrame::AppFrameworkImpl::processYoloTangle(const std::string &jsonString)
{
    // 转换为QJsonObject
    QString qString = QString::fromStdString(jsonString);
    QJsonDocument jsonDocu = QJsonDocument::fromJson(qString.toUtf8());
    QJsonObject jsonObject = jsonDocu.object();
    cv::Mat matImage;
    std::string imageName = jsonObject["imageName"].toString().toStdString();
    int bottomNum = jsonObject["bottomNum"].toInt();
    std::string bottomstr = std::to_string(bottomNum);
    bool isCheck = false;
    Product *pro_;
    for (auto &tempPro : productList_)
    {
        if (tempPro->locateImageName == imageName)
        {
            matImage = tempPro->locateImage;
            LogInfo("tangle timer: get image from algothm");
            isCheck = false;
            pro_ = tempPro;
            break;
        }
        else if (tempPro->locateCheckImageName == imageName)
        {
            matImage = tempPro->locateCheckImage;
            LogInfo("process locateCheck image in tangle");
            isCheck = true;
            pro_ = tempPro;
            break;
        }
    }
    if (matImage.empty())
    {
        LogInfo("imag: {} requre null", imageName);
        return;
    }

    LogInfo("process image: {}", imageName);
    QImage resImg = Utils::matToQImage(matImage);
    // qDebug() << jsonObject["imageName"];
    // 检查是否含有键box
    if (jsonObject.contains("box"))
    {
        QString boxJsonString = jsonObject["box"].toString();
        QJsonArray boxJsonArray = QJsonDocument::fromJson(boxJsonString.toUtf8()).array();

        // 遍历json array
        foreach (const QJsonValue &boxValue, boxJsonArray)
        {
            QJsonObject boxObject = boxValue.toObject();
            QString result = boxObject["result"].toString().toUtf8();

            QString resstr = "tangle; " + result + "; ";
            Json::Value jsParams, jsNum;
            if (isCheck)
            {
                QPainter pp(&resImg);
                QFont font = pp.font();
                font.setPixelSize(50); // 改变字体大小
                font.setFamily("Microsoft YaHei");
                pp.setFont(font);
                pp.setPen(QPen(Qt::red, 5));
                pp.setBrush(QBrush(Qt::red));
                if (result.toInt() < 5 && result.toInt() != 360)
                { // 小于5度定位成功
                    plcDev_->writeDataToDevice("b", "13004", "00", "1");
                    pp.drawText(QPointF(20, 50), "定位成功！");
                }
                else
                {
                    plcDev_->writeDataToDevice("b", "13004", "00", "0");
                    pp.drawText(QPointF(20, 50), "定位失败！");
                }
                plcDev_->writeDataToDevice("n", "12994", "", bottomstr);
            }
            else
            {
                plcDev_->writeDataToDevice("r", "13002", "", result.toStdString());
                plcDev_->writeDataToDevice("n", "12993", "", bottomstr);
                LogInfo("tangle timer: write to plc: 13002_{}, 12993_{}", result.toStdString(), bottomstr);

                QPainter pp(&resImg);
                QFont font = pp.font();
                font.setPixelSize(50); // 改变字体大小
                font.setFamily("Microsoft YaHei");
                pp.setFont(font);
                pp.setPen(QPen(Qt::red, 5));
                pp.setBrush(QBrush(Qt::red));
                pp.drawText(QPointF(20, 50), resstr);
            }
            // cv::putText(*matImage, resstr.toStdString(), cv::Point(5, 30), cv::FONT_HERSHEY_SIMPLEX, 1,
            //             cv::Scalar(0, 0, 255), 2, 8); // 输出文字
        }
        // 1 图像操作：显示在界面、保存
        if (isCheck)
        {
            invokeCpp(mapStorePainter_[DisplayWindows::LocateCheckCamera], "updateImage", Q_ARG(QImage, resImg));
        }
        else
        {
            invokeCpp(mapStorePainter_[DisplayWindows::LocationCamera], "updateImage", Q_ARG(QImage, resImg));
        }

        // cv::imwrite("Utils::getCurrentTime(true)", matImage);
    }
    else
    {
        // 2 算法没有识别到的逻辑: 添加报警信息、数据库中错误瓶数+1
        if (isCheck)
        {
            pro_->locateCheckFalseFlag = true;
            plcDev_->writeDataToDevice("n", "12994", "", jsonObject["bottomNum"].toString().toStdString());
            plcDev_->writeDataToDevice("b", "13004", "00", "0");
        }
        else
        {
            pro_->locateFalseFlag = true;
            plcDev_->writeDataToDevice("n", "12993", "", jsonObject["bottomNum"].toString().toStdString());
            plcDev_->writeDataToDevice("r", "13002", "", "0");
        }
    }
    LogInfo("processYoloTangle finish");
}

void AppFrame::AppFrameworkImpl::processYoloTangleTest(QJsonDocument jsonDocument, cv::Mat matImage)
{
    // 转换为QJsonObject
    QJsonObject jsonObject = jsonDocument.object();
    std::string imageName = jsonObject["imageName"].toString().toStdString();

    // qDebug() << jsonObject["imageName"];
    // 检查是否含有键box
    if (jsonObject.contains("box"))
    {
        QString boxJsonString = jsonObject["box"].toString();
        QJsonArray boxJsonArray = QJsonDocument::fromJson(boxJsonString.toUtf8()).array();

        // 遍历json array
        foreach (const QJsonValue &boxValue, boxJsonArray)
        {
            QJsonObject boxObject = boxValue.toObject();
            QString result = boxObject["result"].toString().toUtf8();
            QString resstr = "tangle; " + result + "; ";
            cv::putText(matImage, resstr.toStdString(), cv::Point(5, 30), cv::FONT_HERSHEY_SIMPLEX, 1,
                        cv::Scalar(0, 0, 255), 2, 8); // 输出文字
        }
        // cv::imwrite("Utils::getCurrentTime(true)", matImage);
        invokeCpp(mapStorePainter_[DisplayWindows::CodeCheckCamera], "updateImage",
                  Q_ARG(QImage, Utils::matToQImage(matImage)));
    }
    else
    {
        // 2 算法没有识别到的逻辑: 添加报警信息、数据库中错误瓶数+1
    }
}

void AppFrame::AppFrameworkImpl::runMainProcess()
{
}

void AppFrame::AppFrameworkImpl::processQrCode(const std::string value)
{
    Product *curProduct = productList_.back();
    LogInfo("read qrCode {}, in {}", value, Utils::getCurrentTime(true));
    if (value == curProduct->qrCodeRes)
    {
        return;
    }
    else
    {
        curProduct->qrCodeRes = value;
        // 如果和上一瓶相同，则是上一瓶没读到，错误的读到此瓶(几乎不可能)
        // if (productList_.size() >= 2)
        // {
        //     --curProduct;
        //     if (curProduct->qrCodeRes == value)
        //     {
        //         curProduct->qrCodeRes = "";
        //         curProduct->logisticsFalseFlag = true;
        //     }
        // }
        permission_->sendQRCode(value);
    }
}

void AppFrame::AppFrameworkImpl::processCode(const std::string code1, const std::string code2)
{
    for (auto &value : productList_)
    {
        if (value->logistics1.empty() && !(value->qrCodeRes.empty()))
        {
            value->logistics1 = code1;
            value->logistics2 = code2;
            break;
        }
    }
}

void AppFrame::AppFrameworkImpl::doPrintCode(uint8_t bottomNum)
{
    for (auto &pro_ : productList_)
    {
        if (pro_->isCode)
        {
            continue;
        }
        if (!pro_->logistics1.empty() && !pro_->logistics2.empty())
        {
            pro_->isCode = true;
        }
        domino_->dominoPrint(pro_->logistics1, pro_->logistics2);
        LogInfo("bottom {}: send data to domino, in {}", bottomNum, Utils::getCurrentTime(true));
    }
}

void AppFrame::AppFrameworkImpl::processPaddleOCR(const std::string &jsonString)
{
    // 找出图像
    LogInfo("recieve paddleOCR algorithm return, in {}", Utils::getCurrentTime(true));
    QString qString = QString::fromStdString(jsonString);
    QJsonDocument jsonDocu = QJsonDocument::fromJson(qString.toUtf8());
    Product *product_;
    for (auto &tempPro : productList_)
    {
        if (!tempPro->logisticsPredict.empty())
        {
            continue;
        }
        if (tempPro->logistics1.empty() || tempPro->logisticsFalseFlag == true)
        {
            continue;
        }
        product_ = tempPro;
    }
    if (product_ = nullptr)
    {
        LogWarn("no match product of ocr");
        return;
    }
    // 转换为QJsonObject
    QJsonObject jsonObject = jsonDocu.object();
    // qDebug() << jsonObject["imageName"];
    // 检查是否含有键box
    if (jsonObject.contains("box"))
    {
        // 定义颜色列表
        static std::vector<cv::Scalar> colorList = {
            cv::Scalar(0, 0, 255),   // 红色
            cv::Scalar(0, 255, 0),   // 绿色
            cv::Scalar(0, 255, 255), // 黄色
            cv::Scalar(255, 0, 255), // 紫色
            cv::Scalar(255, 255, 0), // 青色
            cv::Scalar(0, 165, 255), // 橙色
        };
        int colorIndex = 0;  // 颜色Index
        int dataLocate = 30; // 数据统一显示位置
        // boxstring不是json格式
        QString boxJsonString = jsonObject["box"].toString();
        QJsonArray boxJsonArray = QJsonDocument::fromJson(boxJsonString.toUtf8()).array();

        // 遍历json array
        foreach (const QJsonValue &boxValue, boxJsonArray)
        {
            QJsonObject boxObject = boxValue.toObject();
            QString result = boxObject["result"].toString().toUtf8();
            // todo 物流码是否正确
            if ((product_->logistics1 + product_->logistics2) == result.toStdString())
            {
                plcDev_->writeDataToDevice("b", "13004", "01", "1");
                // 添加生产数据
            }
            else
            {
                plcDev_->writeDataToDevice("b", "13004", "01", "0");
                // 添加生产数据
            }
            LogInfo("result str", result.toStdString());
            QString confidence = boxObject["confidence"].toString();
            float num = confidence.toFloat();
            confidence = QString::number(num, 'f', 2);

            QString lefttop = boxObject["lefttop"].toString();
            QString righttop = boxObject["righttop"].toString();
            QString rightbottom = boxObject["rightbottom"].toString();
            QString leftbottom = boxObject["leftbottom"].toString();

            QJsonArray lefttopArray = QJsonDocument::fromJson(lefttop.toUtf8()).array();
            QJsonArray righttopArray = QJsonDocument::fromJson(righttop.toUtf8()).array();
            QJsonArray rightbottomArray = QJsonDocument::fromJson(rightbottom.toUtf8()).array();
            QJsonArray leftbottomArray = QJsonDocument::fromJson(leftbottom.toUtf8()).array();

            QString resstr = result + "; " + confidence + ";";
            cv::putText(product_->codeCheckImage, resstr.toStdString(), cv::Point(5, dataLocate),
                        cv::FONT_HERSHEY_SIMPLEX, 1, colorList[colorIndex], 2, 8); // 输出文字
            dataLocate += 35;                                                      // 文字换行
            cv::line(product_->codeCheckImage, cv::Point(lefttopArray[0].toInt(), lefttopArray[1].toInt()),
                     cv::Point(righttopArray[0].toInt(), righttopArray[1].toInt()), colorList[colorIndex], 2);
            cv::line(product_->codeCheckImage, cv::Point(righttopArray[0].toInt(), righttopArray[1].toInt()),
                     cv::Point(rightbottomArray[0].toInt(), rightbottomArray[1].toInt()), colorList[colorIndex], 2);
            cv::line(product_->codeCheckImage, cv::Point(rightbottomArray[0].toInt(), rightbottomArray[1].toInt()),
                     cv::Point(leftbottomArray[0].toInt(), leftbottomArray[1].toInt()), colorList[colorIndex], 2);
            cv::line(product_->codeCheckImage, cv::Point(leftbottomArray[0].toInt(), leftbottomArray[1].toInt()),
                     cv::Point(lefttopArray[0].toInt(), lefttopArray[1].toInt()), colorList[colorIndex], 2);

            colorIndex = (colorIndex + 1) % 6; // 颜色轮转
        }

        // 1 图像操作：显示在界面、保存
        // QImage saveImage = Utils::matToQImage(product_->codeCheckImage);
        // Utils::saveImageToFile(saveImage, 2);
        invokeCpp(mapStorePainter_[DisplayWindows::CodeCheckCamera], "updateImage",
                  Q_ARG(QImage, Utils::matToQImage(product_->codeCheckImage)));
    }
    else
    {
        // 2 算法没有识别到的逻辑
        plcDev_->writeDataToDevice("b", "13004", "01", "0");
        // 添加生产数据
    }
    while (productList_.front() != product_)
    {
        // todo 循环清理ng数据
        productList_.pop_front();
    }
    // 清理当前数据
    productList_.pop_front();
}