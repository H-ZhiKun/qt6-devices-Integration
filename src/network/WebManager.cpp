#include "WebManager.h"
#include "Logger.h"

WebManager::WebManager(QObject *parent) : QObject(parent)
{
}

WebManager::~WebManager() noexcept
{
}

void WebManager::init(const YAML::Node &config)
{
    const std::string &urlTangle = config["algorithm"]["ws_tangle"].as<std::string>();
    const std::string &urlTangleCheck = config["algorithm"]["ws_tangleCheck"].as<std::string>();
    const std::string &urlOcr = config["algorithm"]["ws_ocr"].as<std::string>();
    const std::string &type = config["product"]["type"].as<std::string>();
    if (type == "circle")
    {
        tangleClient_ = new WebsocketClient(this);
        tangleCheckClient_ = new WebsocketClient(this);
        ocrClient_ = new WebsocketClient(this);
    }
    else
    {
        ocrClient_ = new WebsocketClient(this);
    }
    if (tangleClient_ != nullptr)
    {
        tangleClient_->connectToServer(urlTangle);
        QObject::connect(tangleClient_, &WebsocketClient::messageReceived,
                         [this](const std::string &data) { processMessage(ALGOType::Tangle, data); });
    }
    if (tangleCheckClient_ != nullptr)
    {
        tangleCheckClient_->connectToServer(urlTangleCheck);
        QObject::connect(tangleCheckClient_, &WebsocketClient::messageReceived,
                         [this](const std::string &data) { processMessage(ALGOType::Check, data); });
    }
    if (ocrClient_ != nullptr)
    {
        ocrClient_->connectToServer(urlOcr);
        QObject::connect(ocrClient_, &WebsocketClient::messageReceived,
                         [this](const std::string &data) { processMessage(ALGOType::OCR, data); });
    }
}

void WebManager::processMessage(const ALGOType modelType, const std::string &data)
{
    Json::Value jsValue;
    std::string result;
    uint32_t bottomNum;
    switch (modelType)
    {
    case ALGOType::Tangle: {
        jsValue = Utils::stringToJson(data);
        bottomNum = jsValue["bottomNum"].asUInt();
        result = jsValue["result"].asString();
        LogInfo("get tangle info: {}", result);
        emit algoRight(modelType, bottomNum, result);
        break;
    }
    case ALGOType::OCR: {
        LogInfo("get ocr info: {}", data);
        jsValue = Utils::stringToJson(data);
        bottomNum = jsValue["bottomNum"].asUInt();
        result = jsValue["result"].asString();
        result = result.substr(0, 24);
        emit algoRight(modelType, bottomNum, result, jsValue["box"].asString());
        break;
    }
    case ALGOType::Check: {
        jsValue = Utils::stringToJson(data);
        bottomNum = jsValue["bottomNum"].asUInt();
        result = jsValue["result"].asString();
        emit algoRight(modelType, bottomNum, result);
        break;
    }
    default:
        break;
    }
    return;
}

void WebManager::sendToALGO(const std::string &typeALGO, const std::string &jsonData, const QByteArray &imageBinaryData)
{
    if (typeALGO == "Location" && tangleClient_)
    {
        tangleClient_->sendData(jsonData, imageBinaryData);
    }
    else if (typeALGO == "OCR" && ocrClient_)
    {
        ocrClient_->sendData(jsonData, imageBinaryData);
    }
    else if (typeALGO == "LocateCheck" && tangleCheckClient_)
    {
        tangleCheckClient_->sendData(jsonData, imageBinaryData);
    }
}

bool WebManager::isConnected(const ALGOType type) const
{
    bool ret = false;
    switch (type)
    {
    case ALGOType::Tangle: {
        if (tangleClient_ != nullptr)
            ret = tangleClient_->getConnect();
        break;
    }
    case ALGOType::Check: {
        if (tangleCheckClient_ != nullptr)
            ret = tangleCheckClient_->getConnect();
        break;
    }
    case ALGOType::OCR: {
        if (ocrClient_ != nullptr)
            ret = ocrClient_->getConnect();
        break;
    }
    default:
        break;
    }
    return ret;
}
