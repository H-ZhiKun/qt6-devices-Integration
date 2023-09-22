#include "WebManager.h"

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
    const std::string &urlOcr = config["algorithm"]["url_ocr"].as<std::string>();
    tangleClient_ = new WebsocketClient(this);
    tangleCheckClient_ = new WebsocketClient(this);
    tangleClient_->connectToServer(urlTangle);
    tangleCheckClient_->connectToServer(urlTangleCheck);
    QObject::connect(tangleClient_, &WebsocketClient::messageReceived,
                     [this](const std::string &data) { emit tangleRecv(data); });
    QObject::connect(tangleCheckClient_, &WebsocketClient::messageReceived,
                     [this](const std::string &data) { emit tangleCheckRecv(data); });
}

void WebManager::sendToALGO(const uint8_t typeALGO, const std::string &jsonData, const QByteArray &imageBinaryData)
{
    switch (typeALGO)
    {
    case 0: {
        if (tangleClient_)
            tangleClient_->sendData(jsonData, imageBinaryData);
        break;
    }
    case 1: {
        if (ocrClient_)
            ocrClient_->sendData(jsonData, imageBinaryData);
        break;
    }
    case 2: {
        if (tangleCheckClient_)
            tangleCheckClient_->sendData(jsonData, imageBinaryData);
        break;
    }
    default:
        break;
    }
}
