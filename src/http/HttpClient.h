#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>
#include <QString>
#include <QUrl>

class HttpClient : public QObject
{
    Q_OBJECT

  public:
    explicit HttpClient(QObject *parent = nullptr);
  public slots:
    void sendGetRequest(const std::string &urlStr);
    void sendPostRequest(const std::string &urlStr, const std::string &postDataStr);

  private slots:
    void handleReply();

  private:
    QNetworkAccessManager *m_manager;

  signals:
    void responseReceived(const std::string &response);
};

#endif // HTTPCLIENT_H
