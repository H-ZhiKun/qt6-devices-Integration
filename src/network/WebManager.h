#ifndef WEBMANAGER_H
#define WEBMANAGER_H
#include "WebsocketClient.h"
#include <QObject>
#include <Utils.h>
#include <cstdint>
#include <json/json.h>
#include <yaml-cpp/yaml.h>
struct OcrRectangle
{
    OcrRectangle(std::string result, uint16_t lefttopx, uint16_t lefttopy, uint16_t leftbottomx, uint16_t leftbottomy,
                 uint16_t righttopx, uint16_t righttopy, uint16_t rightbottomx, uint16_t rightbottomy)
        : result(result), lefttopx(lefttopx), lefttopy(lefttopy), leftbottomx(leftbottomx), leftbottomy(leftbottomy),
          righttopx(righttopx), righttopy(righttopy), rightbottomx(rightbottomx), rightbottomy(rightbottomy)
    {
    }
    std::string result;
    uint16_t lefttopx;
    uint16_t lefttopy;
    uint16_t leftbottomx;
    uint16_t leftbottomy;
    uint16_t righttopx;
    uint16_t righttopy;
    uint16_t rightbottomx;
    uint16_t rightbottomy;
};

enum class ALGOType
{
    Tangle = 0,
    OCR,
    Check
};
class WebManager : public QObject
{
    Q_OBJECT
  public:
    explicit WebManager(QObject *parent = nullptr);
    virtual ~WebManager() noexcept;
    void init(const YAML::Node &config);
    bool isConnected(const ALGOType type) const;
  public slots:
    void sendToALGO(const std::string &, const std::string &jsonData, const QByteArray &imageBinaryData);
    void processMessage(const ALGOType, const std::string &);
  signals:
    void algoRight(const ALGOType, const uint32_t bottleNum, const std::string &result, const std::string &area = "");

  private:
    WebsocketClient *ocrClient_{nullptr};
    WebsocketClient *tangleClient_{nullptr};
    WebsocketClient *tangleCheckClient_{nullptr};
};

#endif // WEBMANAGER_H