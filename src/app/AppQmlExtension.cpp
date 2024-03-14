#include "AppQmlExtension.h"
#include "AlertWapper.h"
#include "AlgorithmWapper.h"
#include "AppMetaFlash.h"
#include "BasePLCDevice.h"
#include "BaseProduct.h"
#include "CameraManager.h"
#include "Cognex.h"
#include "ElecWapper.h"
#include "FormulaWapper.h"
#include "LogWapper.h"
#include "Logger.h"
#include "Permission.h"
#include "PrinterDevice.h"
#include "ProductWapper.h"
#include "StatisticsWapper.h"
#include "UserWapper.h"
#include "Utils.h"
#include "WebManager.h"
#include <cstdint>
#include <vector>

std::string AppFrame::AppQmlExtension::deleteSqlData(const std::string &value)
{
    bool ret = false;
    Json::Value jsTemp = Utils::stringToJson(value);
    if (jsTemp["tableName"] == "formula")
    {
        std::string name = jsTemp["name"].asString();
        if (!name.empty())
        {
            ret = FormulaWapper::deleteFormula(name.c_str());
        }
    }
    else if (jsTemp["tableName"] == "user")
    {
        std::string id = jsTemp["id"].asString();
        if (!id.empty())
        {
            ret = UserWapper::deleteUser(id.c_str());
        }
    }
    return Utils::makeResponse(ret);
}

std::string AppFrame::AppQmlExtension::modifySqlData(const std::string &value)
{
    bool ret = false;
    Json::Value jsTemp = Utils::stringToJson(value);
    std::string tableName = jsTemp["tableName"].asString();
    jsTemp.removeMember("tableName");
    std::string jsonStr = Utils::jsonToString(jsTemp);
    if (tableName == "formula")
    {
        ret = FormulaWapper::modifyFormula(jsonStr.c_str());
    }
    else if (tableName == "user")
    {
        ret = UserWapper::modifyUser(jsonStr.c_str());
    }
    else if (tableName == "algorithm")
    {
        ret = AlgorithmWapper::modifyAlgorithm(jsonStr.c_str());
    }
    return Utils::makeResponse(ret);
}

std::string AppFrame::AppQmlExtension::insertSqlData(const std::string &value)
{
    bool ret = false;
    Json::Value jsTemp = Utils::stringToJson(value);
    std::string tableName = jsTemp["tableName"].asString();
    jsTemp.removeMember("tableName");
    std::string jsonStr = Utils::jsonToString(jsTemp);
    if (tableName == "formula")
    {
        ret = FormulaWapper::insertFormula(jsonStr.c_str());
    }
    else if (tableName == "user")
    {
        ret = UserWapper::insertUser(jsonStr.c_str());
    }
    else if (tableName == "log")
    {
        ret = LogWapper::insertLog(jsonStr.c_str());
    }
    return Utils::makeResponse(ret);
}

std::string AppFrame::AppQmlExtension::selectSqlData(const std::string &value)
{
    bool ret = false;
    Json::Value jsTemp = Utils::stringToJson(value);
    QString jsRet;
    if (jsTemp["tableName"] == "formula")
    {
        std::string name = jsTemp["name"].asString();
        if (!name.empty())
        {
            jsRet = FormulaWapper::selectOneFormula(name.c_str());
            if (!jsRet.isEmpty())
            {
                ret = true;
            }
        }
    }
    else if (jsTemp["tableName"] == "user")
    {
        std::string name = jsTemp["name"].asString();
        if (!name.empty())
        {
            jsRet = UserWapper::selectUser(name.c_str());
            if (!jsRet.isEmpty())
            {
                ret = true;
            }
        }
    }
    else if (jsTemp["tableName"] == "userID")
    {
        Json::Value jsParams;
        std::string jsRet = UserWapper::selectUserID(value.c_str()).toStdString();
        if (!jsRet.empty())
        {
            ret = true;
            jsParams["id"] = jsRet;
        }
        return Utils::makeResponse(ret, std::move(jsParams));
    }
    else
    {
        jsRet = selectPage(value).c_str();
        ret = true;
    }
    return Utils::makeResponse(ret, std::move(Utils::stringToJson(jsRet.toStdString())));
}

std::string AppFrame::AppQmlExtension::selectPage(const std::string &value)
{
    bool ret = false;
    Json::Value jsAlertVal, jsReturn;
    int num;
    Json::Value jsParams = Utils::stringToJson(value);
    PageIndex pageType = appMetaFlash().getPageIndex(jsParams["tableName"].asString());
    if (jsParams["tableName"] == "alarm")
    {
        jsAlertVal = AlertWapper::selectAlertDataPaged(jsParams["pageSize"].asInt(), jsParams["pageNumber"].asInt(), "",
                                                       "id desc");
        jsReturn["num"] = AlertWapper::alertNum();
    }
    else if (jsParams["tableName"] == "log")
    {
        jsAlertVal =
            LogWapper::selectLogDataPaged(jsParams["pageSize"].asInt(), jsParams["pageNumber"].asInt(), "", "id desc");
        jsReturn["num"] = LogWapper::LogNum();
    }
    else if (jsParams["tableName"] == "bottom")
    {
        jsAlertVal = ProductWapper::selectProductDataPaged(jsParams["pageSize"].asInt(), jsParams["pageNumber"].asInt(),
                                                           "created_time::date = CURRENT_DATE", "id desc");
        jsReturn["num"] = ProductWapper::productNum();
    }
    else if (jsParams["tableName"] == "statistics")
    {
        jsAlertVal = StatisticsWapper::selectDataPaged(jsParams["pageSize"].asInt(), jsParams["pageNumber"].asInt(), "",
                                                       "id desc");
        jsReturn["num"] = StatisticsWapper::dataCount();
    }
    if (jsAlertVal.empty())
    {
        return std::string();
    }

    for (Json::Value &jsonSingleValue : jsAlertVal)
    {
        if (!jsonSingleValue["created_time"].empty())
        {
            std::string datetimeStr = jsonSingleValue["created_time"].asString();
            // 替换T为空格
            jsonSingleValue["created_time"] = Utils::transferDataFromSql(jsonSingleValue["created_time"].asString());
        }
        if (jsParams["tableName"] == "statistics" && !jsonSingleValue["updated_time"].empty())
        {
            std::string datetimeStr = jsonSingleValue["updated_time"].asString();
            // 替换T为空格
            jsonSingleValue["updated_time"] = Utils::transferDataFromSql(jsonSingleValue["updated_time"].asString());
        }
        if (jsParams["tableName"] == "alarm")
        {
            if (!jsonSingleValue["updated_time"].empty())
            {
                jsonSingleValue["updated_time"] =
                    Utils::transferDataFromSql(jsonSingleValue["updated_time"].asString());
            }
            if (jsonSingleValue["content"] == "设备电源报警" && jsonSingleValue["state"] == "true")
            {
                Json::Value jsMainVal;
                std::string powerOffStr = Utils::makeResponse(ret, Json::Value(), "powerOff");
                appFramework().invokeCpp(&appMetaFlash(), "runtimeRoutine", PageIndex::PageHome,
                                         QString::fromStdString(powerOffStr));
            }
        }
    }
    appFramework().invokeCpp(&appMetaFlash(), "runtimeRoutine", pageType,
                             QString::fromStdString(Utils::jsonToString(jsAlertVal)));
    return Utils::jsonToString(jsReturn);
}

std::string AppFrame::AppQmlExtension::readPLC(const std::string &value)
{
    Utils::asyncTask([value] {
        const auto &plc = appFramework().getPlugin<BasePLCDevice>();
        bool ret = false;
        Json::Value jsParams = Utils::stringToJson(value);
        Json::Value res;
        PageIndex pageName = appMetaFlash().getPageIndex(jsParams["pageName"].asString());
        std::string funcName = jsParams["funcName"].asString();
        jsParams.removeMember("pageName");
        jsParams.removeMember("funcName");
        for (const auto &key : jsParams.getMemberNames())
        {
            auto temp = plc->readDevice(key);
            if (!temp.empty())
            {
                res[key] = temp;
                ret = true;
            }
        }
        std::string retJson = Utils::makeResponse(ret, std::move(res), std::move(funcName));
        appFramework().invokeCpp(&appMetaFlash(), "runtimeRoutine", pageName, QString::fromStdString(retJson));
    });
    return std::string("");
}

/**
 * @description: 例：key:M071_b_12288_14  count=4   value  "1"或"0"
 *               例：key:M071_r_12288  count=3   value    "0.76"
 * @param {string} &value 传入Json数组
 * @return {*}
 */
std::string AppFrame::AppQmlExtension::writePLC(const std::string &value)
{
    Utils::asyncTask([value] {
        const auto &plc = appFramework().getPlugin<BasePLCDevice>();
        bool ret = true;
        LogInfo("write to plc: {}", value);

        Json::Value jsParams = Utils::stringToJson(value);
        std::string funcName;
        PageIndex pageName;
        if (jsParams.isMember("pageName"))
        {
            funcName = jsParams["funcName"].asString();
            pageName = appMetaFlash().getPageIndex(jsParams["pageName"].asString());
            jsParams.removeMember("funcName");
            jsParams.removeMember("pageName");
            // 修改方瓶、 圆瓶
            if (pageName == PageIndex::PageFormula && jsParams.isMember("bottoleType"))
            {
                const auto &product = appFramework().getPlugin<BaseProduct>();
                if (jsParams["bottoleType"] == "2")
                {
                    product->setBottleShape(BottleShape::Square);
                }
                else
                {
                    product->setBottleShape(BottleShape::Round);
                }
            }
        }
        for (const auto &key : jsParams.getMemberNames())
        {
            const auto &value = jsParams[key].asString();
            auto bWriteRet = plc->writeDevice(key, value);
            if (!bWriteRet)
            {
                LogInfo("write plc error addr={},value={}", key, value);
                ret = false;
                break;
            }
        }
        if (!funcName.empty())
        {
            funcName = ret ? (funcName + "成功!") : (funcName + "失败!");
            std::string retJson = Utils::makeResponse(ret, Json::Value{}, std::move(funcName));
            appFramework().invokeCpp(&appMetaFlash(), "runtimeRoutine", pageName, QString::fromStdString(retJson));
        }
    });
    return "";
}

/**
 * @description: 自动登录接口，包括两个功能：
 *               1 选取某个账户，下次会自动登录
 *               2 取消以后的自动登录
 * @param {string} &str 分“1”和“0”，对应上述两种功能
 * @return {*}
 */
std::string AppFrame::AppQmlExtension::autoLogin(const std::string &str)
{
    bool ret = false;
    Json::Value jsElecVal;
    std::string result;
    if (str == "1")
    { // 自动登录
        jsElecVal = UserWapper::selectLoginUser();
        if (jsElecVal.empty())
        {
            result = Utils::makeResponse(ret);
        }
        else
        {
            ret = true;
            result = Utils::makeResponse(ret, std::move(jsElecVal));
        }
    }
    else if (str == "0")
    { // 取消自动登录
        ret = UserWapper::cancelAutoLogin();
        result = Utils::makeResponse(ret);
        isLogin_ = true;
    }
    return result;
}

std::string AppFrame::AppQmlExtension::zeroClearing(const std::string &)
{
    const auto &product = appFramework().getPlugin<BaseProduct>();
    const auto &plc = appFramework().getPlugin<BasePLCDevice>();

    bool bRet = true;
    if (product->getType() == TypeProduct::TypeCircle)
    {
        bRet = plc->writeBitValue(0, 12992, 1, 1);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        bRet = plc->writeBitValue(0, 12992, 1, 0);
    }
    else
    {
        bRet = plc->writeBitValue(64, 140, 0, 1);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        plc->writeBitValue(64, 140, 0, 0);
    }
    if (!bRet)
    {
        LogInfo("clear plc status error");
    }
    return Utils::makeResponse(bRet);
}

std::string AppFrame::AppQmlExtension::refreshMainPage(const std::string &)
{
    if (!isLogin_)
    {
        return std::string("");
    }
    Utils::asyncTask([] {
        const auto &product = appFramework().getPlugin<BaseProduct>();
        const auto &cameraManager = appFramework().getPlugin<CameraManager>();
        const auto &cognex = appFramework().getPlugin<Cognex>();
        const auto &permission = appFramework().getPlugin<Permission>();
        const auto &plcDev = appFramework().getPlugin<BasePLCDevice>();
        const auto &printerDevice = appFramework().getPlugin<PrinterDevice>();
        const auto &webManager = appFramework().getPlugin<WebManager>();
        bool ret = true;
        Json::Value jsMainVal;
        // 状态显示信息
        jsMainVal["cognexState"] = cognex->getConnect();
        jsMainVal["permissionState"] = permission->getConnect();
        jsMainVal["plcState"] = plcDev->isConnected();
        jsMainVal["textProduceState"] = "未获取信息";
        jsMainVal["textEquipmentSteps"] = "未获取信息";
        jsMainVal["imageTangleAlgo"] = webManager->isConnected(ALGOType::Tangle);
        jsMainVal["imageCheckAlgo"] = webManager->isConnected(ALGOType::Check);
        jsMainVal["imageOCRAlgo"] = webManager->isConnected(ALGOType::OCR);
        std::string textProduceState = "";
        std::string textEquipmentSteps = "";
        if (product->getType() == TypeProduct::TypeCircle || product->getType() == TypeProduct::TypeLine)
        {
            jsMainVal["painterState"] = printerDevice->getConnect();
        }

        jsMainVal["textProduceState"] = plcDev->getDeviceStatus(PLCStateType::PLCStates);
        jsMainVal["textEquipmentSteps"] = plcDev->getDeviceStatus(PLCStateType::PLCSteps);

        // 正在报警的信息
        jsMainVal["alarmText"] = AlertWapper::selectAlarmingData();
        // 相机状态
        std::vector<std::string> cameraState = cameraManager->cameraState();
        for (const auto &key : cameraState)
        {
            jsMainVal["image" + key] = true;
        }
        Json::Value jsCount = ProductWapper::selectCountData();
        for (const auto &member : jsCount.getMemberNames())
        {
            jsMainVal[member] = jsCount[member];
        }
        std::string resJson = Utils::makeResponse(ret, std::move(jsMainVal), "refreshMainPage");
        // 调用"runtimeRoutine"函数，传入PageHome和jsonSingleValue
        appFramework().invokeCpp(&appMetaFlash(), "runtimeRoutine", PageIndex::PageHome,
                                 QString::fromStdString(resJson));
        // 刷新主界面瓶子数据
        selectProduct();
    });

    return Utils::makeResponse(true);
}

/**
 * @description: 刷新生产数据界面
 * @param {string} &
 * @return {*}
 */
std::string AppFrame::AppQmlExtension::refreshPowerPage(const std::string &sqlFlag)
{
    Utils::asyncTask([sqlFlag] {
        const auto &plcDev = appFramework().getPlugin<BasePLCDevice>();
        const auto &product = appFramework().getPlugin<BaseProduct>();

        Json::Value jsPowerVal = ProductWapper::selectDetailCountData();
        bool ret = true;
        if (product->getType() == TypeProduct::TypeCircle)
        {
            auto readValues = plcDev->readArray<float>(12590, 11);
            if (readValues.size() > 0)
            {
                jsPowerVal["a_phase_voltage"] = readValues[0];      // A相电压
                jsPowerVal["b_phase_voltage"] = readValues[1];      // B相电压
                jsPowerVal["c_phase_voltage"] = readValues[2];      // C相电压
                jsPowerVal["a_direction_current"] = readValues[3];  // A向电流
                jsPowerVal["b_direction_current"] = readValues[4];  // B向电流
                jsPowerVal["c_direction_current"] = readValues[5];  // C向电流
                jsPowerVal["total_active_power"] = readValues[6];   // 总有功功率
                jsPowerVal["total_apparent_power"] = readValues[7]; // 总视在功率
                jsPowerVal["total_active_energy"] = readValues[8];  // 总有功电能
                jsPowerVal["temperature"] = readValues[9];          // 温度
                jsPowerVal["humidity"] = readValues[10];            // 湿度
            }
        }
        else
        {
            auto readValues = plcDev->readArray<float>(20, 13);
            if (readValues.size() > 0)
            {
                jsPowerVal["a_phase_voltage"] = readValues[0];        // 电能表A相电压
                jsPowerVal["b_phase_voltage"] = readValues[1];        // 电能表B相电压
                jsPowerVal["c_phase_voltage"] = readValues[2];        // 电能表C相电压
                jsPowerVal["a_direction_current"] = readValues[3];    // 电能表A相电流
                jsPowerVal["b_direction_current"] = readValues[4];    // 电能表B相电流
                jsPowerVal["c_direction_current"] = readValues[5];    // 电能表C相电流
                jsPowerVal["total_active_power"] = readValues[6];     // 电能表总有功功率
                jsPowerVal["total_apparent_power"] = readValues[7];   // 电能表总视在功率
                jsPowerVal["combined_active_energy"] = readValues[8]; // 电能表组合有功总电能
                jsPowerVal["positive_active_energy"] = readValues[9]; // 电能表正向有功电能
                jsPowerVal["reverse_active_energy"] = readValues[10]; // 电能表反向有功电能
                jsPowerVal["temperature"] = readValues[11];           // 设备温度
                jsPowerVal["humidity"] = readValues[12];              // 设备湿度
            }
        }
        // 每一分钟写入数据库，定时器设置在qml中，通过sqlFlag标志位控制
        if (sqlFlag == "1")
        {
            ElecWapper::insertData(jsPowerVal);
        }
        std::string resJson = Utils::makeResponse(ret, std::move(jsPowerVal));
        appFramework().invokeCpp(&appMetaFlash(), "runtimeRoutine", PageIndex::PageElectric,
                                 QString::fromStdString(resJson));
    });
    return Utils::makeResponse(true);
}

std::string AppFrame::AppQmlExtension::produceReset(const std::string &)
{
    Utils::asyncTask([] {
        const auto &product = appFramework().getPlugin<BaseProduct>();
        const auto &cameraManager = appFramework().getPlugin<CameraManager>();
        const auto &cognex = appFramework().getPlugin<Cognex>();
        const auto &printerDevice = appFramework().getPlugin<PrinterDevice>();
        const auto &plcDev = appFramework().getPlugin<BasePLCDevice>();
        product->clearQueue();
        printerDevice->resetLvPrintCodes();
        plcDev->zeroClear();
        if (product->getType() != TypeProduct::TypeCircle)
        {
            plcDev->writeArray<uint8_t>(64, 136, std::vector<uint8_t>(8, 0));
            plcDev->writeBitValue(64, 140, 0, 1);
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            plcDev->writeBitValue(64, 140, 0, 0);
        }
        else
        { // 回转式
            plcDev->writeArray<uint16_t>(0, 13005, std::vector<uint16_t>(36, 0));
            plcDev->writeBitValue(0, 12992, 5, 1);
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            plcDev->writeBitValue(0, 12992, 5, 0);
        }
    });
    return std::string();
}

/**
 * @description:初始化配方界面的配方列表和用户界面的用户列表
 * @param {string} &type
 * @return {*}
 */
std::string AppFrame::AppQmlExtension::initPageData(const std::string &type)
{
    Json::Value jsAlertVal;
    PageIndex pageType = appMetaFlash().getPageIndex(type);
    if (type == "formula")
    {
        jsAlertVal = FormulaWapper::selectAllFormula();
    }
    else if (type == "user")
    {
        jsAlertVal = UserWapper::selectAllUser();
    }
    else if (type == "algorithm")
    {
        jsAlertVal = AlgorithmWapper::selectAlgorithmData();
    }
    for (Json::Value &jsonSingleValue : jsAlertVal)
    {
        appFramework().invokeCpp(&appMetaFlash(), "runtimeRoutine", pageType,
                                 QString::fromStdString(Utils::jsonToString(jsonSingleValue)));
    }
    return std::string("");
}

/**
 * @description: 选取数据库中前5个瓶子，传到qml界面
 * @param {string} &
 * @return {*}
 */
std::string AppFrame::AppQmlExtension::selectProduct(const std::string &)
{
    bool ret = false;
    Json::Value jsProduct = ProductWapper::selectProductDataPage();
    if (!jsProduct.empty())
    {
        ret = true;
        for (Json::Value &jsonSingleValue : jsProduct)
        {
            std::string qrCode = jsonSingleValue["qr_code"].asString();
            std::string ocrRes = jsonSingleValue["ocr_result"].asString();
            if (qrCode.size() > 25)
            {
                qrCode = qrCode.substr(25);
            }
            if (ocrRes.size() > 13)
            {
                ocrRes.insert(12, 1, '\n');
                if (ocrRes.size() > 25)
                {
                    ocrRes = ocrRes.substr(0, 25);
                }
            }
            jsonSingleValue["ocr_result"] = ocrRes;
            jsonSingleValue["qr_code"] = qrCode;
            std::string logistic = jsonSingleValue["logistics"].asString();
            logistic.insert(logistic.size() / 2, 1, '\n');
            jsonSingleValue["logistics"] = logistic;
        }
        std::string resJson = Utils::makeResponse(ret, std::move(jsProduct), "selectProduct");
        // 调用"runtimeRoutine"函数，传入PageHome和jsonSingleValue
        appFramework().invokeCpp(&appMetaFlash(), "runtimeRoutine", PageIndex::PageHome,
                                 QString::fromStdString(resJson));
    }
    return std::string("");
}

/**
 * @description: 自动下载配方，包括两种功能：
 *               添加需要自动下载的配方（记录进数据库）和自动下载配方，
 *               使用jsParams["autoLoad"]参数控制
 * @param {string} &value 传入的Json数组
 * @return {*}
 */
std::string AppFrame::AppQmlExtension::autoLoad(const std::string &value)
{
    bool ret = false;
    Json::Value jsParams = Utils::stringToJson(value);
    Json::Value writeJson, loadValue, oneValue;
    std::string result;
    if (jsParams["autoLoad"] == "1")
    { // 自动下载
        const auto &product = appFramework().getPlugin<BaseProduct>();
        // 1 selectdata 选择自动下载的配方数据
        loadValue = PgsqlHelper::getSqlHelper().selectData("formula_data", "auto_load = true", "");
        for (auto it = loadValue.begin(); it != loadValue.end(); it++)
        {
            oneValue = *it;
            break;
        }
        // 2 根据产品选择不同的下载plc点位
        if (jsParams["type"] == "circle")
        { // 回转式
            writeJson["deviceProduceSpeedInput"] = oneValue["speed_produce"];
            writeJson["deviceproduceAccelerationInput"] = oneValue["acceleration_produce"];
            writeJson["deviceproduceDecelerationInput"] = oneValue["deceleration_produce"];
            writeJson["codeXPosition"] = oneValue["code_x_position"];
            writeJson["codeYPosition"] = oneValue["code_y_position"];
            writeJson["bottoleType"] = oneValue["bottle_type"];
            writeJson["formulaFin"] = "1"; // 通知plc可以启动
            // 修改方瓶、 圆瓶
            if (oneValue["bottle_type"] == "2")
            {
                product->setBottleShape(BottleShape::Square);
            }
            else
            {
                product->setBottleShape(BottleShape::Round);
            }
        }
        else
        { // 直线式和瓶盖检测
            writeJson["deviceProduceSpeedInput"] = oneValue["speed_produce"];
            writeJson["deviceproduceAccelerationInput"] = oneValue["acceleration_produce"];
            writeJson["deviceproduceDecelerationInput"] = oneValue["deceleration_produce"];
            writeJson["materialDiameter"] = oneValue["material_diameter"];
        }

        writePLC(Utils::jsonToString(writeJson));
        zeroClearing("");
        ret = true;
    }
    else
    { // 修改名配方为自动下载
        // 1 取消其他配方的自动下载
        ret = FormulaWapper::cancelAutoLoad();
        // 2 修改该配方为自动下载
        writeJson["name"] = jsParams["name"];
        writeJson["auto_load"] = "1";
        std::string writeJsonStr = Utils::jsonToString(writeJson);
        FormulaWapper::modifyFormula(writeJsonStr.c_str());
    }
    return Utils::makeResponse(ret, std::move(oneValue));
}

std::string AppFrame::AppQmlExtension::powerOff(const std::string &value)
{
    // appFramework().quitProgram(); // 关闭程序
    // Utils::closeComputer("15");   // 15秒后关机
    return "";
}

std::string AppFrame::AppQmlExtension::getPLCCount(const std::string &data)
{
    const auto &plc = appFramework().getPlugin<BasePLCDevice>();
    const auto &cameraManager = appFramework().getPlugin<CameraManager>();
    const auto &cognex = appFramework().getPlugin<Cognex>();
    Json::Value countJson;
    countJson["in"] = plc->getPlcCount(PLCCount::InCount);
    countJson["out"] = plc->getPlcCount(PLCCount::OutCount);
    countJson["camera"] = plc->getPlcCount(PLCCount::CameraCount);
    countJson["qrcode"] = plc->getPlcCount(PLCCount::QRCodeCount);
    countJson["code"] = plc->getPlcCount(PLCCount::CodeCount);
    countJson["reciveCamera"] = cameraManager->getCameraCount("OCR");
    countJson["reciveCognex"] = cognex->getCount();
    return Utils::makeResponse(true, std::move(countJson), "getPLCCount");
}

std::string AppFrame::AppQmlExtension::getDataReport(const std::string &data)
{
    return std::string();
}
