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
    registerExpectation(ExpectedFunction::RefreshMainPage, std::bind(&AppFrameworkImpl::refreshMainPage, this));
    registerExpectation(ExpectedFunction::RefreshPowerPage, std::bind(&AppFrameworkImpl::refreshPowerPage, this));

    registerExpectation(ExpectedFunction::RefreshElecData,
                        std::bind(&AppFrameworkImpl::refreshElecData, this)); // 注册直线式电能表数据
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
    initProduct();
    initNetworkClient();
    initBaumerManager();
    initPLC();
    // runtime task
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
    return Utils::makeResponse(ret, std::move(res));
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
            ret = plcDev_->writeDevice(vKeys[1], vKeys[2], "0", curValue);
            if (ret == false)
            {
                LogWarn("write data faile! address: {}", value);
            }
        }
        else if (vKeys.size() == 4)
        {
            ret = plcDev_->writeDevice(vKeys[1], vKeys[2], vKeys[3], curValue);
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
std::string AppFrame::AppFrameworkImpl::refreshElecData()
{
    bool ret = true;
    Json::Value jsElecVal;
    jsElecVal["a_phase_voltage"] = plcDev_->readDevice("di", "0043");        // 电能表A相电压
    jsElecVal["b_phase_voltage"] = plcDev_->readDevice("di", "0045");        // 电能表B相电压
    jsElecVal["c_phase_voltage"] = plcDev_->readDevice("di", "0047");        // 电能表C相电压
    jsElecVal["a_direction_current"] = plcDev_->readDevice("di", "0049");    // 电能表A相电流
    jsElecVal["b_direction_current"] = plcDev_->readDevice("di", "0051");    // 电能表B相电流
    jsElecVal["c_direction_current"] = plcDev_->readDevice("di", "0053");    // 电能表C相电流
    jsElecVal["total_active_power"] = plcDev_->readDevice("di", "0055");     // 电能表总有功功率
    jsElecVal["total_apparent_power"] = plcDev_->readDevice("di", "0057");   // 电能表总视在功率
    jsElecVal["combined_active_energy"] = plcDev_->readDevice("di", "0059"); // 电能表组合有功总电能
    jsElecVal["positive_active_energy"] = plcDev_->readDevice("di", "0061"); // 电能表正向有功电能
    jsElecVal["reverse_active_energy"] = plcDev_->readDevice("di", "0063");  // 电能表反向有功电能
    jsElecVal["temperature"] = plcDev_->readDevice("di", "0065");            // 设备温度
    jsElecVal["humidity"] = plcDev_->readDevice("di", "0067");               // 设备湿度

    jsElecVal["textCountAll"] = plcDev_->readDevice("di", "0009");           // 进料数量
    jsElecVal["textCountInspection"] = plcDev_->readDevice("di", "0011");    // 检测数量
    jsElecVal["textCountEliminate"] = plcDev_->readDevice("di", "0013");     // 总剔除数量
    jsElecVal["textCountLogisticscode"] = plcDev_->readDevice("di", "0015"); // 物流码剔除数量

    std::string result = Utils::makeResponse(ret, std::move(jsElecVal));
    return result;
}

std::string AppFrame::AppFrameworkImpl::refreshMainPage()
{
    bool ret = true;
    Json::Value jsMainVal;
    jsMainVal["image0"] = "0";
    jsMainVal["image1"] = "0";
    jsMainVal["image2"] = "0";
    jsMainVal["dominoState"] = std::to_string(domino_->getConnect());
    jsMainVal["cognexState"] = std::to_string(cognex_->getConnect());
    jsMainVal["permissionState"] = std::to_string(permission_->getConnect());
    jsMainVal["plcState"] = std::to_string(plcDev_->getConnect());
    jsMainVal["textProduceState"] = plcDev_->readDevice("n", "12612");

    jsMainVal["textEquipmentSteps"] = plcDev_->readDevice("n", "12613");
    std::vector<uint8_t> cameraState = baumerManager_->cameraState();
    for (uint8_t i = 0; i < cameraState.size(); i++)
    {
        jsMainVal["image" + std::to_string(cameraState[i])] = "1";
    }
    std::string result = Utils::makeResponse(ret, std::move(jsMainVal));
    return result;
}

std::string AppFrame::AppFrameworkImpl::refreshPowerPage()
{
    Json::Value jsPowerVal;
    bool ret = true;
    jsPowerVal["positive_active_energy"] = plcDev_->readDevice("r", "12586"); // 正向有功电能
    jsPowerVal["reverse_active_energy"] = plcDev_->readDevice("r", "12588");  // 反向有功电能
    jsPowerVal["a_phase_voltage"] = plcDev_->readDevice("r", "12590");        // A相电压
    jsPowerVal["b_phase_voltage"] = plcDev_->readDevice("r", "12592");        // B相电压
    jsPowerVal["c_phase_voltage"] = plcDev_->readDevice("r", "12594");        // C相电压
    jsPowerVal["temperature"] = plcDev_->readDevice("r", "12608");            // 温度
    jsPowerVal["total_active_power"] = plcDev_->readDevice("r", "12602");     // 总有功功率
    jsPowerVal["total_apparent_power"] = plcDev_->readDevice("r", "12604");   // 总视在功率
    jsPowerVal["total_active_energy"] = plcDev_->readDevice("r", "12606");    // 总有功电能
    jsPowerVal["a_direction_current"] = plcDev_->readDevice("r", "12596");    // A向电流
    jsPowerVal["b_direction_current"] = plcDev_->readDevice("r", "12598");    // B向电流
    jsPowerVal["c_direction_current"] = plcDev_->readDevice("r", "12600");    // C向电流
    jsPowerVal["humidity"] = plcDev_->readDevice("r", "12610");               // 湿度
    std::string result = Utils::makeResponse(ret, std::move(jsPowerVal));
    return result;
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

    QObject::connect(webManager_, &WebManager::ocrRecv, [this](const std::string &json) { processOCR(json); });
    QObject::connect(webManager_, &WebManager::tangleRecv, [this](const std::string &json) { processTangle(json); });
    QObject::connect(webManager_, &WebManager::tangleCheckRecv,
                     [this](const std::string &json) { processTangleCheck(json); });

    // 获取到二维码并发送
    QObject::connect(cognex_, &Cognex::ReadQRCode, [this](const std::string &value) { whenCognexRecv(value); });
    // 获取到物流码并存储
    QObject::connect(permission_, &Permission::codeRight,
                     [this](const uint16_t number, const std::string &des, const std::string &code1,
                            const std::string &code2) { whenPermissionRecv(number, des, code1, code2); });
    LogInfo("network client start success.");
}

void AppFrame::AppFrameworkImpl::initPLC()
{
    plcDev_ = new PLCDevice;
    plcDev_->init(config_);
    QObject::connect(plcDev_, &PLCDevice::bottleMove, [this](const uint64_t bottomNum) { whenBottomMove(bottomNum); });
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
    invokeCpp(&AppMetaFlash::instance(), AppMetaFlash::instance().invokeRuntimeRoutine,
              Q_ARG(PageIndex, PageIndex::PageMain), Q_ARG(QString, Utils::jsonToString(jsMainVal).c_str()));
}

void AppFrame::AppFrameworkImpl::updateProduceRealData()
{
    Json::Value jsProduceVal;
    /*生产数据界面实时更新数据*/

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

void AppFrame::AppFrameworkImpl::updateByMinute(const int minute)
{
    // todo:电能信息写入数据库、上报
}

void AppFrame::AppFrameworkImpl::updateByDay(const int year, const int month, const int day)
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
    strTangleResultPath_ = saveImageDir + "/LocationCameraResult/";
    strTangleCheckResultPath_ = saveImageDir + "/LocateCheckCameraResult/";
    if (!qdir.exists(strTanglePath_.c_str()))
    {
        bool res = qdir.mkdir(strTanglePath_.c_str());
        if (!res)
        {
            LogWarn("create Location dir file!");
        }
    }
    if (!qdir.exists(strOcrPath_.c_str()))
    {
        bool res = qdir.mkdir(strOcrPath_.c_str());
        if (!res)
        {
            LogWarn("create OcrPath dir file!");
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
    if (!qdir.exists(strTangleResultPath_.c_str()))
    {
        bool res = qdir.mkdir(strTangleResultPath_.c_str());
        if (!res)
        {
            LogWarn("create TangleResultPath dir file!");
        }
    }
    if (!qdir.exists(strTangleCheckResultPath_.c_str()))
    {
        bool res = qdir.mkdir(strTangleCheckResultPath_.c_str());
        if (!res)
        {
            LogWarn("create TangleCheckResultPath dir file!");
        }
    }
}

void AppFrame::AppFrameworkImpl::initProduct()
{
    circleProduct_ = new CircleProduct();
}

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
    if (circleProduct_)
    {
        delete circleProduct_;
        circleProduct_ = nullptr;
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
        while (bThreadHolder) // 线程退出Flag
        {
            // 实时捕获摄像头采集
            cv::Mat mat;
            mat = baumerManager_->getCamaeraMat(0);
            if (!mat.empty())
            {
                afterCaputureImage(0, mat);
                LogInfo("product process:image locate get.");
                invokeCpp(mapStorePainter_[DisplayWindows::LocationCamera], "updateImage",
                          Q_ARG(QImage, Utils::matToQImage(mat)));
            }
            mat = baumerManager_->getCamaeraMat(1);
            if (!mat.empty())
            {
                afterCaputureImage(1, mat);
                LogInfo("product process:image code get.");
                invokeCpp(mapStorePainter_[DisplayWindows::CodeCheckCamera], "updateImage",
                          Q_ARG(QImage, Utils::matToQImage(mat)));
            }
            mat = baumerManager_->getCamaeraMat(2);
            if (!mat.empty())
            {
                afterCaputureImage(2, mat);
                LogInfo("product process:image locate check get.");
                invokeCpp(mapStorePainter_[DisplayWindows::LocateCheckCamera], "updateImage",
                          Q_ARG(QImage, Utils::matToQImage(mat)));
            }
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

void AppFrame::AppFrameworkImpl::sendOneToAlgo()
{
    std::string jsonData;
    QByteArray byteArray;
    Utils::makeJsonAndByteArray(cv::Mat(800, 800, CV_8UC3, cv::Scalar(255, 255, 255)), 0, "init", "tangle",
                                strTanglePath_, jsonData, byteArray);
    webManager_->sendToALGO(0, jsonData, byteArray);
}

void AppFrame::AppFrameworkImpl::drawText(QImage &img, const QString &text)
{
    QPainter pp(&img);
    QFont font = pp.font();
    font.setPixelSize(50); // 改变字体大小
    font.setFamily("Microsoft YaHei");
    pp.setFont(font);
    pp.setPen(QPen(Qt::red, 5));
    pp.setBrush(QBrush(Qt::red));
    pp.drawText(QPointF(20, 50), text);
}

void AppFrame::AppFrameworkImpl::whenBottomMove(const uint64_t number)
{
    Utils::asyncTask([this, number] {
        if (circleProduct_ == nullptr)
            return;

        circleProduct_->newProduct(number);

        // PLC工位从1开始计数，软件工位从0开始计数，以下工位都是软件工位

        // 电机旋转工位=5 下发定位在旋转前=5
        const auto rotate = circleProduct_->getIndex(5);

        // 打码工位=9 下发复合定位在打码前=8
        const auto locateCheck = circleProduct_->getIndex(8);

        // 打码工位=9 收到进入打码工位信号立刻下发数据到打印机=9
        // const auto printer = circleProduct_->getIndex(9);

        // 打码复合工位=14 考虑图片接受时延+算法时延=16
        const auto codeCheck = circleProduct_->getIndex(16);
        if (rotate && !rotate->locateResult.empty())
        {
            plcDev_->writeDevice("r", "13002", "", rotate->locateResult);
            plcDev_->writeDevice("n", "12993", "", std::to_string(rotate->numBottom));
            LogInfo("product process:write plc:number={},value={}.", rotate->numBottom, rotate->locateResult);
        }
        if (locateCheck)
        {
            if (locateCheck->logistics1.empty())
            {
                plcDev_->writeDevice("b", "13004", "0", "0");
            }
            else
            {
                // plcDev_->writeDataToDevice("b", "13004", "0", locateCheck->locateCheckResult);
                plcDev_->writeDevice("b", "13004", "00", "1");
                plcDev_->writeDevice("n", "12994", "", std::to_string(locateCheck->numBottom));
                LogInfo("product process:locateCheck:number={},value={}.", locateCheck->numBottom,
                        locateCheck->locateCheckResult);
                invokeCpp(domino_, "dominoPrint", Q_ARG(std::string, locateCheck->logistics1),
                          Q_ARG(std::string, locateCheck->logistics2));
                LogInfo("product process:print:number={},code1={},code2={}.", locateCheck->numBottom,
                        locateCheck->logistics1, locateCheck->logistics2);
            }
        }
        // if (printer && !printer->logistics1.empty() && printer->locateCheckResult == "1")
        // {
        //     invokeCpp(domino_, "dominoPrint", Q_ARG(std::string, printer->logistics1),
        //               Q_ARG(std::string, printer->logistics2));
        //     LogInfo("product process:print:number={},code1={},code2={}.", printer->numBottom, printer->logistics1,
        //             printer->logistics2);
        // }
        if (codeCheck)
        {
            plcDev_->writeDevice("b", "13004", "1", codeCheck->OCRResult);

            // 这里应该做流程结束保存数据记录的工作和清理工位。
            // 保存到数据库
            // todo
            // 清理工位
            circleProduct_->completeProduct();
        }
    });
}

void AppFrame::AppFrameworkImpl::whenCognexRecv(const std::string &code)
{
    Utils::asyncTask([this, code] {
        if (circleProduct_ == nullptr)
            return;
        if (code == "no read")
        {
            // 失败逻辑
        }
        uint16_t number = circleProduct_->updateQRCode(code);
        if (number)
            invokeCpp(permission_, "sendQRCode", Q_ARG(const uint16_t, number), Q_ARG(std::string, code));
    });
}

void AppFrame::AppFrameworkImpl::whenPermissionRecv(const uint16_t number, const std::string &describtion,
                                                    const std::string &code1, const std::string &code2)
{
    if (circleProduct_ == nullptr)
        return;
    circleProduct_->updateLogistics(number, describtion, code1, code2);
}

void AppFrame::AppFrameworkImpl::afterCaputureImage(const uint8_t windId, const cv::Mat &mat)
{
    Utils::asyncTask([this, windId, image = mat.clone()] {
        if (circleProduct_ == nullptr)
            return;
        uint32_t bottomNum = 0;
        std::string filePath;
        std::string modelName;
        QString currentDateTimeStr = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss_zzz");
        switch (windId)
        {
        case 0: {
            cv::Mat newMat;
            cv::resize(image, newMat, {800, 800});
            modelName = "tangle";
            bottomNum = plcDev_->getFIFOInfo().numPosition;
            filePath = strTanglePath_ + currentDateTimeStr.toStdString() + ".jpg";
            circleProduct_->updateLocate(image, filePath);
            break;
        }
        case 1: {
            modelName = "ocr";
            bottomNum = plcDev_->getFIFOInfo().numVerifyCoding;
            filePath = strOcrPath_ + currentDateTimeStr.toStdString() + ".jpg";
            circleProduct_->updateOCR(image, filePath);
            break;
        }
        case 2: {
            modelName = "tangleCheck";
            bottomNum = plcDev_->getFIFOInfo().numVerifyPos;
            filePath = strTangleCheckPath_ + currentDateTimeStr.toStdString() + ".jpg";
            circleProduct_->updateLocateCheck(image, filePath);
            break;
        }
        default:
            break;
        }
        std::string sendJson;
        QByteArray sendBytes;
        LogInfo("product process:send to algo:number={},model={},bytes={}.", bottomNum, windId, sendBytes.size());
        Utils::makeJsonAndByteArray(image, bottomNum, "", modelName, filePath, sendJson, sendBytes);
        invokeCpp(webManager_, "sendToALGO", Q_ARG(uint8_t, windId), Q_ARG(std::string, sendJson),
                  Q_ARG(QByteArray, sendBytes));
    });
}

void AppFrame::AppFrameworkImpl::processOCR(const std::string &jsonData)
{
    Utils::asyncTask([this, jsonData] {
        if (circleProduct_ == nullptr)
            return;
        Json::Value jsValue = Utils::stringToJson(jsonData);
        uint32_t bottomNum = jsValue["bottomNum"].asUInt();
        const auto ptrBottom = circleProduct_->getNumber(bottomNum);
    });
}

void AppFrame::AppFrameworkImpl::processTangle(const std::string &jsonData)
{
    Utils::asyncTask([this, jsonData] {
        if (circleProduct_ == nullptr)
            return;
        Json::Value jsValue = Utils::stringToJson(jsonData);
        uint32_t bottomNum = jsValue["bottomNum"].asUInt();
        const auto ptrBottom = circleProduct_->getNumber(bottomNum);
        if (ptrBottom == nullptr)
        {
            LogInfo("product process:recv from tangle:number not found.");
            return;
        }
        cv::Mat mat = ptrBottom->locateImage;
        if (mat.empty())
        {
            LogInfo("product process:recv from tangle:number={},mat is null.", bottomNum);
            return;
        }
        std::string result = "0";
        jsValue = Utils::stringToJson(jsValue["box"].asString());
        for (const auto &item : jsValue)
        {
            result = item["result"].asString();
        }
        int tangleResult = std::atoi(result.c_str());
        tangleResult = (tangleResult + 98) % 360;
        circleProduct_->updateLocateResult(bottomNum, std::to_string(tangleResult));
        result = "tangle; " + result + "; ";
        QImage Image = Utils::matToQImage(mat);
        drawText(Image, result.c_str());
        QString currentDateTimeStr = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss_zzz");
        QByteArray byteArray;
        QBuffer buffer(&byteArray);
        buffer.open(QIODevice::WriteOnly);
        Image.save(&buffer, "jpg");
        Utils::saveImageToFile(byteArray, strTangleResultPath_ + currentDateTimeStr.toStdString() + ".jpg");
        invokeCpp(mapStorePainter_[DisplayWindows::LocationCamera], "updateImage", Q_ARG(QImage, Image));
    });
}

void AppFrame::AppFrameworkImpl::processTangleCheck(const std::string &jsonData)
{
    Utils::asyncTask([this, jsonData] {
        if (circleProduct_ == nullptr)
            return;
        Json::Value jsValue = Utils::stringToJson(jsonData);
        uint32_t bottomNum = jsValue["bottomNum"].asUInt();
        const auto ptrBottom = circleProduct_->getNumber(bottomNum);
    });
}