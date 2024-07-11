#include "CentralClient.h"

CentralClient::CentralClient(QObject *parent) : TCPClient(parent)
{
}

CentralClient::~CentralClient()
{
}

void CentralClient::dealing(std::vector<unsigned char> buffer)
{
}

void CentralClient::sendProductData(const Json::Value &jsonValue)
{
    std::string productDataStr = Utils::jsonToString(jsonValue);
    QByteArray byteArray = QByteArray(productDataStr.c_str(), static_cast<int>(productDataStr.size()));
    std::cout << "send control system data: " << productDataStr << std::endl;
    sendData(byteArray);
}