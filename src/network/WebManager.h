#ifndef WEBMANAGER_H
#define WEBMANAGER_H
#include "WebsocketClient.h"
#include <QObject>
#include <vector>
#include <yaml-cpp/yaml.h>
class WebManager : public QObject
{
    Q_OBJECT
  public:
    explicit WebManager(QObject *parent = nullptr);
    virtual ~WebManager() noexcept;
    void init(const YAML::Node &config);
  public slots:
    void sendToALGO(const uint8_t windId, const std::string &jsonData, const QByteArray &imageBinaryData);
  signals:
    void ocrRecv(const std::string &);
    void tangleRecv(const std::string &);
    void tangleCheckRecv(const std::string &);

  private:
    WebsocketClient *ocrClient_{nullptr};
    WebsocketClient *tangleClient_{nullptr};
    WebsocketClient *tangleCheckClient_{nullptr};
};

#endif // WEBMANAGER_H