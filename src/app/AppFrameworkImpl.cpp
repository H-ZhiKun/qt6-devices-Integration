#include "AppFrameworkImpl.h"
#include "AlertWapper.h"
#include "AppFramework.h"
#include "AppMetaFlash.h"
#include "CameraWapper.h"
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
    registerExpectation(ExpectedFunction::GetCameraList,
                        std::bind(&AppFrameworkImpl::getCameraList, this, std::placeholders::_1));
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
    // 保证一定完成了资源清理
    memoryClean();
}

int AppFrame::AppFrameworkImpl::run()
{
    loadConfig();
    // 初始化日志记录器
    CLogger::GetLogger().initLogger(qApp->applicationDirPath().toStdString() + "/logs/log_.html", spdlog::level::debug,
                                    10, 5);

    LogInfo("AppFrame Run");
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

std::string AppFrame::AppFrameworkImpl::getCameraList(const std::string &value)
{
    Json::Value jsRet;
    bool ret = false;
    auto ids = baumerManager_->getCameraList();
    for (auto &id : ids)
    {
        jsRet["cameraId"].append(id);
        ret = true;
    }
    return Utils::makeResponse(ret, std::move(jsRet));
}

std::string AppFrame::AppFrameworkImpl::getCameraParam(const std::string &value)
{
    bool ret = false;
    auto params = Utils::stringToJson(value);
    std::string snNumber = params["sn"].asString();
    Json::Value jsVal = baumerManager_->getCameraParam(snNumber);
    if (!jsVal.isNull())
    {
        std::shared_lock lock(mtxSNPainter_);
        jsVal["qml_window"] = -1;
        for (auto &[wnd, cameraId] : mapWndDisplay_)
        {
            if (snNumber == cameraId)
            {
                jsVal["qml_window"] = static_cast<uint16_t>(wnd);
            }
        }
        ret = true;
    }
    else
    {
        return "";
    }
    return Utils::makeResponse(ret, std::move(jsVal));
}

std::string AppFrame::AppFrameworkImpl::setCameraParam(const std::string &value)
{
    bool ret = false;
    Json::Value jsParams = Utils::stringToJson(value);
    if (!jsParams.isNull() && jsParams.isMember("sn_num"))
    {
        std::string snNum = jsParams["sn_num"].asString();
        uint16_t window = jsParams["qml_window"].asUInt();
        bindDisplay(snNum, static_cast<DisplayWindows>(window));
        auto rs1 = baumerManager_->setCameraParam(jsParams);
        auto jsStore = baumerManager_->getCameraParam(snNum);
        jsStore["qml_window"] = window;
        auto rs2 = CameraWapper::modifyCamera(jsStore);
        ret = rs1 && rs2;
    }
    return Utils::makeResponse(ret);
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
    try
    {
        std::string filePath = qApp->applicationDirPath().toStdString() + "/config.yaml";
        config_ = YAML::LoadFile(filePath);
        LogInfo("loadConfig success.");
        qDebug() << config_["app"]["database"]["host"].as<std::string>();
    }
    catch (const YAML::Exception &e)
    {
        LogError("Error parsing YAML: {}", e.what());
        Utils::appExit(-1);
    }
}

void AppFrame::AppFrameworkImpl::initSqlHelper()
{
    if (!PgsqlHelper::getSqlHelper().initSqlHelper())
    {
        LogInfo("sqlhelper init failed.");
        memoryClean();
        Utils::appExit(-1);
    }
    Json::Value jsVal = CameraWapper::selectAllCamera();
    if (!jsVal.isNull())
    {
        for (auto &jsItem : jsVal)
        {
            std::string key = jsItem["sn_num"].asString();
            std::string value = jsItem["qml_window"].asString();
            if (!value.empty())
            {
                mapWndDisplay_[static_cast<DisplayWindows>(std::stoi(value))] = key;
            }
        }
    }
    updateFormulaData(); // 放在异步处理中会丢失
    updateUserData();
}

void AppFrame::AppFrameworkImpl::initNetworkClient()
{
    domino_ = new Domino();
    domino_->startClient("127.0.0.1", 20001);
    cognex_ = new Cognex();
    cognex_->startClient("192.168.101.111", 23);
    permission_ = new Permission();
    permission_->startClient("127.0.0.1", 61000);
    httpClient_ = new HttpClient();
    QObject::connect(httpClient_, &HttpClient::responseReceived, [this](const std::string &json) {
        if (json.empty())
        {
            LogError("http network error.");
        }
        else
        {
            processHttpRes(json);
        }
    });
}

void AppFrame::AppFrameworkImpl::initPLC()
{
    plcDev_ = new PLCDevice;
    plcDev_->init();
    // QObject::connect(plcDev_, &PLCDevice::bottomMove, [this]() {
    //     // 瓶位移信号逻辑
    //     Product *productNew = new Product();
    //     // 检测是否空瓶
    //     if (1)
    //     {
    //     }
    //     else
    //     {
    //         productNew->setHasBottom(false);
    //     }
    //     productManager_->qrCodeQueue.push(productNew);
    // });
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
    jsProduceVal["positive_active_energy"] = "3.22"; // 正向有功电能
    jsProduceVal["reverse_active_energy"] = "4.22";  // 反向有功电能
    jsProduceVal["a_phase_voltage"] = "3.45";        // A相电压
    jsProduceVal["b_phase_voltage"] = "3.54";        // B相电压
    jsProduceVal["c_phase_voltage"] = "3.36";        // C相电压
    jsProduceVal["temperature"] = "45.7";            // 温度
    jsProduceVal["total_active_power"] = "2045";     // 总有功功率
    jsProduceVal["total_apparent_power"] = "5424";   // 总视在功率
    jsProduceVal["total_active_energy"] = "54.5";    // 总有功电能
    jsProduceVal["a_direction_current"] = "12.4";    // A向电流
    jsProduceVal["b_direction_current"] = "15.3";    // B向电流
    jsProduceVal["c_direction_current"] = "14.5";    // C向电流
    jsProduceVal["humidity"] = "91.3%";              // 湿度

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
    std::shared_lock lock(mtxSNPainter_);
    for (const auto &[key, value] : mapWndDisplay_)
    {
        auto camId = key;
        std::string sn = value;
        if (sn.empty())
        {
            invokeCpp(mapStorePainter_[camId], "stopPainting");
            continue;
        }
        std::list<cv::Mat> matData = baumerManager_->getImageBySN(sn);
        if (matData.size() == 0)
        {
            continue;
        }
        cv::Mat temp = matData.back();
        Utils::asyncTask([this, camId, target = std::move(temp)] {
            const FIFOInfo &it = plcDev_->getFIFOInfo();
            int num = it.numPosition;
            std::string url;
            if (camId == DisplayWindows::CodeCheckCamera)
            {
                url = "http://192.168.101.8:5001/paddleOCR";
                LogInfo("CodeCheckCamera bottom: ", num);
            }
            else if (camId == DisplayWindows::LocationCamera)
            {
                url = "http://192.168.101.8:5000/predict_tangle";
                LogInfo("LocationCamera bottom: ", num);
            }
            else if (camId == DisplayWindows::LocateCheckCamera)
            {
                url = "http://192.168.101.8:5000/predict_tangle";
                LogInfo("LocateCheckCamera bottom: ", num);
            }
            invokeCpp(httpClient_, "sendPostRequest", Q_ARG(std::string, url),
                      Q_ARG(std::string, Utils::makeHttpBodyWithCVMat(target, num)));
            QImage img = Utils::matToQImage(target);
            if (img.isNull() == false)
            {
                if (saveImageFlag.load(std::memory_order_acquire))
                {
                    saveImageFlag.store(false, std::memory_order_release);
                    saveImageToFile(img, camId);
                }
                invokeCpp(mapStorePainter_[camId], "updateImage", Q_ARG(QImage, img));
            }
        });
    }
}

void AppFrame::AppFrameworkImpl::updateByMinute(const std::string &minute)
{
    // 每分钟更新电能数据
    QList<QVariantMap> dataList;
    Json::Value dataEle;
    // 与PLC通信得到数据
    float num = atoi(minute.c_str());
    dataEle["id"] = 0;
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
    std::list<std::string> fields{"\"id\" SERIAL PRIMARY KEY",
                                  "\"qr_code_reslut\" varchar(256) COLLATE utf8_bin",
                                  "\"logistics_code_gt\" char(24) COLLATE utf8_bin",
                                  "\"locate_camera_image\" varchar(256) COLLATE utf8_bin",
                                  "\"locate_res\" real",
                                  "\"locate_check_camera_image\" varchar(256) COLLATE utf8_bin",
                                  "\"locate_check_res\" boolean",
                                  "\"code_check_camera_image\" varchar(256) COLLATE utf8_bin",
                                  "\"logistics_code\" char(24) COLLATE utf8_bin",
                                  "\"logistics_code_res\" boolean",
                                  "\"batch_num\" varchar(256) COLLATE utf8_bin",
                                  "\"formula_name\" varchar(128) COLLATE utf8_bin",
                                  "\"created_time\" timestamp DEFAULT CURRENT_TIMESTAMP",
                                  "UNIQUE (\"id\")"};
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
    std::list<std::string> lastFields{"\"id\" SERIAL PRIMARY KEY",
                                      "\"qr_code_reslut\" varchar(256) COLLATE utf8_bin",
                                      "\"logistics_code_gt\" char(24) COLLATE utf8_bin",
                                      "\"locate_camera_image\" varchar(256) COLLATE utf8_bin",
                                      "\"locate_res\" real",
                                      "\"locate_check_camera_image\" varchar(256) COLLATE utf8_bin",
                                      "\"locate_check_res\" boolean",
                                      "\"code_check_camera_image\" varchar(256) COLLATE utf8_bin",
                                      "\"logistics_code\" char(24) COLLATE utf8_bin",
                                      "\"logistics_code_res\" boolean",
                                      "\"batch_num\" varchar(256) COLLATE utf8_bin",
                                      "\"formula_name\" varchar(128) COLLATE utf8_bin",
                                      "\"created_time\" timestamp DEFAULT CURRENT_TIMESTAMP",
                                      "UNIQUE (\"id\")"};
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
    Json::Value jsVal = CameraWapper::selectAllCamera();
    baumerManager_ = new BaumerManager(jsVal);
    baumerManager_->start();
}

// void AppFrame::AppFrameworkImpl::initHttp()
// {
//     HttpApiManager *httpLogistics = new HttpApiManager();
//     HttpApiManager *httpLocate = new HttpApiManager();
//     HttpApiManager *httpLocateCheck = new HttpApiManager();
//     http_.emplace_back(httpLogistics);
//     http_.emplace_back(httpLocate);
//     http_.emplace_back(httpLocateCheck);
//     QObject::connect(http_[0], &HttpApiManager::requestFinished,
//                      [this](const QString &response, cv::Mat matImage, const int bottomNum) {
//                          if (response.isEmpty())
//                          {
//                              return;
//                          }
//                          LogInfo("resJS", response.toStdString());
//                          // 1 获取方框位置，绘制
//                          QJsonDocument jsonDocument = QJsonDocument::fromJson(response.toUtf8());
//                          // QVariantMap dataMap = jsonDocumentData.toVariant().toMap();
//                          // ocr:识别物流码  yolovn5_tagle：定位  yolov5：定位复核
//                          //  检查是否解析成功
//                          if (!jsonDocument.isNull())
//                          {
//                              Utils::asyncTask([this, jsonDocument = std::move(jsonDocument),
//                                                matImage = std::move(matImage), bottomNum]() {
//                                  processPaddleOCR(std::move(jsonDocument), std::move(matImage), bottomNum);
//                              });
//                          }
//                      });
//     QObject::connect(http_[1], &HttpApiManager::requestFinished,
//                      [this](const QString &response, cv::Mat matImage, const int bottomNum) {
//                          if (response.isEmpty())
//                          {
//                              return;
//                          }
//                          LogInfo("resJS", response.toStdString());
//                          // 1 获取方框位置，绘制
//                          QJsonDocument jsonDocument = QJsonDocument::fromJson(response.toUtf8());
//                          //  检查是否解析成功
//                          if (!jsonDocument.isNull())
//                          {
//                              processYoloTangle(jsonDocument, matImage, bottomNum);
//                          }
//                      });
//     QObject::connect(http_[2], &HttpApiManager::requestFinished,
//                      [this](const QString &response, cv::Mat matImage, const int bottomNum) {
//                          if (response.isEmpty())
//                          {
//                              return;
//                          }
//                          LogInfo("resJS", response.toStdString());
//                          QJsonDocument jsonDocument = QJsonDocument::fromJson(response.toUtf8());
//                          if (!jsonDocument.isNull())
//                          {
//                              processYoloTangle(jsonDocument, matImage, bottomNum);
//                          }
//                      });
// }

void AppFrame::AppFrameworkImpl::initFile()
{
    QDir qdir;
    saveImageDir = qdir.currentPath() + "/" + "Image";
    if (!qdir.exists(saveImageDir))
    {
        bool res = qdir.mkdir(saveImageDir);
        if (!res)
        {
            LogWarn("create saveImageDir dir file!");
        }
    }
    QString saveImageSubDir1 = saveImageDir + "/LocationCamera";
    QString saveImageSubDir2 = saveImageDir + "/CodeCheckCamera";
    QString saveImageSubDir3 = saveImageDir + "/LocateCheckCamera";
    if (!qdir.exists(saveImageSubDir1))
    {
        bool res = qdir.mkdir(saveImageSubDir1);
        if (!res)
        {
            LogWarn("create LocationCamera dir file!");
        }
    }
    if (!qdir.exists(saveImageSubDir2))
    {
        bool res = qdir.mkdir(saveImageSubDir2);
        if (!res)
        {
            LogWarn("create CodeCheckCamera dir file!");
        }
    }
    if (!qdir.exists(saveImageSubDir3))
    {
        bool res = qdir.mkdir(saveImageSubDir3);
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

void AppFrame::AppFrameworkImpl::bindDisplay(const std::string &snId, const DisplayWindows &painterId)
{
    std::unique_lock lock(mtxSNPainter_);
    auto iter = mapWndDisplay_.begin();
    for (; iter != mapWndDisplay_.end(); ++iter)
    {
        if (iter->second == snId)
        {
            iter->second = "";
        }
    }
    mapWndDisplay_[painterId] = snId;
}

void AppFrame::AppFrameworkImpl::memoryClean()
{
    // 退出所有的子线程并回收线程栈资源，堆资源需要后续手动释放
    bThreadHolder = false;
    mapWndDisplay_.clear();
    mapStorePainter_.clear();
    std::this_thread::sleep_for(std::chrono::milliseconds(5000)); // 等到子线程回收资源
    for (auto &ptr : lvFulltimeThread_)
    {
        ptr.join();
    }
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
    if (baumerManager_ != nullptr)
    {
        delete baumerManager_;
        baumerManager_ = nullptr;
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
    if (httpClient_ != nullptr)
    {
        delete httpClient_;
        httpClient_ = nullptr;
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
            //  Utils::asyncTask([this] { updateSensorRealData(); });
            //  Utils::asyncTask([this] { updateValveRealData(); });
            //  Utils::asyncTask([this] { updatePowerRealData(); });
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
                // int num = plcDev_->getFIFOInfo().numPosition;
                // Json::Value jsParams, jsNum;
                // jsParams["tangle_r_13002"] = "53";
                // jsNum["tangle_n_12993"] = std::to_string(num);
                // writePLC(Utils::jsonToString(jsParams));
                // writePLC(Utils::jsonToString(jsNum));
            }
            if (second != recSecond)
            { // 更新每秒数据
                recSecond = second;
                // invokeCpp(cognex_, cognex_->scanOnce()); // cognex调用方法
                // invokeCpp(domino_, "dominoPrint", Q_ARG(std::string, year + month + day + second + "A"),
                //           Q_ARG(std::string, year + month + day + second + "B")); // 打码机调用方法
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    }));

    lvFulltimeThread_.push_back(std::thread([this] {
        // 视频渲染线程
        while (bThreadHolder)
        {
            updateVideo();
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }));
}

void AppFrame::AppFrameworkImpl::processHttpRes(const std::string &jsonData)
{
    // 调用yolo或者ocr处理过程
    Json::Value jsVal = Utils::stringToJson(jsonData);
    std::string type = jsVal["type"].asString();
    if (type == "")
    {
        // Utils::asyncTask([this] { processPaddleOCR(); });
    }
    else if (type == "")
    {
        // Utils::asyncTask([this] { processYoloTangle(); });
    }
    else if (type == "")
    {
        // Utils::asyncTask([this] { processYoloTangle(); });
    }
}

void AppFrame::AppFrameworkImpl::processYoloTangle(QJsonDocument &jsonDocument, cv::Mat &matImage, const int bottomNum)
{
    // 转换为QJsonObject
    QJsonObject jsonObject = jsonDocument.object();
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
            jsParams["tangle_r_13002"] = result.toStdString();
            jsNum["tangle_n_12993"] = std::to_string(bottomNum);
            writePLC(Utils::jsonToString(jsParams));
            writePLC(Utils::jsonToString(jsNum));
            LogInfo("writePLC 13002: ", result.toStdString());
            LogInfo("writePLC 12993: ", bottomNum);
            cv::putText(matImage, resstr.toStdString(), cv::Point(5, 30), cv::FONT_HERSHEY_SIMPLEX, 1,
                        cv::Scalar(0, 0, 255), 2, 8); // 输出文字
        }
        // 1 图像操作：显示在界面、保存
        invokeCpp(mapStorePainter_[DisplayWindows::LocateCheckCamera], "updateImage",
                  Q_ARG(QImage, Utils::matToQImage(matImage)));
        // cv::imwrite("Utils::getCurrentTime(true)", matImage);
    }
    else
    {
        // 2 算法没有识别到的逻辑: 添加报警信息、数据库中错误瓶数+1
    }
}

void AppFrame::AppFrameworkImpl::runMainProcess()
{
    // while (1 == plcDev_->shift.load())
    // {
    //     if (productManager_->qrCodeQueue.size() > ProductManager::getQRCodeMaxSize())
    //     {
    //         // 二维码队列满，取出一个进行打码
    //         Product *qrProduct = productManager_->qrCodeQueue.front();
    //         productManager_->qrCodeQueue.pop();
    //         // 定位检测点队列
    //         productManager_->locateQueue.push(qrProduct);
    //         // 以下异步执行
    //         // 是否有瓶
    //         if (qrProduct->getHasBottom())
    //         {
    //             // 发送二维码读取信号并于防伪系统做比较
    //         }
    //         else
    //         {
    //             // 不发送二维码读取信号
    //         }
    //     }
    // }
}

void AppFrame::AppFrameworkImpl::processPaddleOCR(QJsonDocument jsonDocument, cv::Mat matImage, const int bottomNum)
{
    // 转换为QJsonObject
    QJsonObject jsonObject = jsonDocument.object();
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
            cv::putText(matImage, resstr.toStdString(), cv::Point(5, dataLocate), cv::FONT_HERSHEY_SIMPLEX, 1,
                        colorList[colorIndex], 2, 8); // 输出文字
            dataLocate += 35;                         // 文字换行
            cv::line(matImage, cv::Point(lefttopArray[0].toInt(), lefttopArray[1].toInt()),
                     cv::Point(righttopArray[0].toInt(), righttopArray[1].toInt()), colorList[colorIndex], 2);
            cv::line(matImage, cv::Point(righttopArray[0].toInt(), righttopArray[1].toInt()),
                     cv::Point(rightbottomArray[0].toInt(), rightbottomArray[1].toInt()), colorList[colorIndex], 2);
            cv::line(matImage, cv::Point(rightbottomArray[0].toInt(), rightbottomArray[1].toInt()),
                     cv::Point(leftbottomArray[0].toInt(), leftbottomArray[1].toInt()), colorList[colorIndex], 2);
            cv::line(matImage, cv::Point(leftbottomArray[0].toInt(), leftbottomArray[1].toInt()),
                     cv::Point(lefttopArray[0].toInt(), lefttopArray[1].toInt()), colorList[colorIndex], 2);

            colorIndex = (colorIndex + 1) % 6; // 颜色轮转
        }

        // 1 图像操作：显示在界面、保存
        std::string imagePath = "D:/deviceintegration/build/Debug/image/test2.jpg";
        QImage saveImage = Utils::matToQImage(matImage);
        saveImageToFile(saveImage, DisplayWindows::LocateCheckCamera);
        if (jsonObject["model"] == "paddleOCR")
            invokeCpp(mapStorePainter_[DisplayWindows::CodeCheckCamera], "updateImage",
                      Q_ARG(QImage, Utils::matToQImage(matImage)));
        else if (jsonObject["model"] == "predict_tangle")
            invokeCpp(mapStorePainter_[DisplayWindows::LocationCamera], "updateImage",
                      Q_ARG(QImage, Utils::matToQImage(matImage)));
    }
    else
    {
        // 2 算法没有识别到的逻辑: 添加报警信息、数据库中错误瓶数+1
    }
}

void AppFrame::AppFrameworkImpl::saveImageToFile(QImage &imgSave, const DisplayWindows &camId)
{
    Utils::asyncTask([this, imgSave, camId] {
        QString currentDateTimeStr = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss_zzz");
        QString saveImageSubDir = "";
        switch (camId)
        {
        case DisplayWindows::LocationCamera:
            saveImageSubDir = saveImageDir + "/LocationCamera/";
            break;
        case DisplayWindows::CodeCheckCamera:
            saveImageSubDir = saveImageDir + "/CodeCheckCamera/";
            break;
        case DisplayWindows::LocateCheckCamera:
            saveImageSubDir = saveImageDir + "/LocateCheckCamera/";
            break;
        }
        QString imagePath = saveImageSubDir + currentDateTimeStr + ".jpg";
        if (!imgSave.save(imagePath, "JPG"))
        {
            qDebug() << "save image failed : " + imagePath;
        }
    });
}
