#ifndef HTTPAPIMANAGER_H
#define HTTPAPIMANAGER_H

#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QUrl>
#include <opencv2/core/core.hpp>
class HttpApiManager : public QObject
{
    Q_OBJECT

  public:
    explicit HttpApiManager(QObject *parent = nullptr);
    ~HttpApiManager() = default;
    void post(const QString &url, const QString &data, cv::Mat &matImage);

  signals:
    void requestFinished(const QString &response, cv::Mat matImage);

  private slots:
    void onReplyFinished(QNetworkReply *reply, cv::Mat matImage);

  private:
    QNetworkAccessManager m_networkManager;
};

#endif // HTTPAPIMANAGER_H
