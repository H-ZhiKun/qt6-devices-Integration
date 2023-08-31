#include "WebManager.h"

WebManager::WebManager(QObject *parent) : QObject(parent)
{
}

WebManager::~WebManager() noexcept
{
}

void WebManager::init(const YAML::Node &config)
{
    std::string urlTangle = config["algorithm"]["url_predict"].as<std::string>();
    std::string urlOcr = config["algorithm"]["url_ocr"].as<std::string>();
    tangleClient_ = new WebsocketClient(this);
    tangleClient_->connectToServer(urlTangle);
    QObject::connect(tangleClient_, &WebsocketClient::messageReceived,
                     [this](const std::string &data) { emit tangleRecv(data); });
}

void WebManager::sendToALGO(uint8_t windId, const std::string &jsonData, const QByteArray &imageBinaryData)
{
    switch (windId)
    {
    case 0: {
        tangleClient_->sendData(jsonData, imageBinaryData);
        break;
    }
    case 1: {
        ocrClient_->sendData(jsonData, imageBinaryData);
        break;
    }
    case 2: {
        tangleCheckClient_->sendData(jsonData, imageBinaryData);
        break;
    }
    default:
        break;
    }
}
