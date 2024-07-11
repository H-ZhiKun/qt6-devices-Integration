#pragma once
#include "BaseProduct.h"
#include "TCPClient.h"
#include "Utils.h"
#include <vector>

class CentralClient : public TCPClient
{
    Q_OBJECT
  public:
    explicit CentralClient(QObject *parent = nullptr);
    virtual ~CentralClient();
    virtual void dealing(std::vector<unsigned char> buffer) override;
    void sendProductData(const Json::Value &jsonValue);
};
