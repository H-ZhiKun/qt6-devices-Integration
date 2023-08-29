#include "HttpClient.h"

HttpClient::HttpClient(QObject *parent) : QObject(parent), m_manager(new QNetworkAccessManager(this))
{
}

void HttpClient::sendGetRequest(const std::string &urlStr)
{
    QUrl url = QUrl::fromUserInput(QString::fromStdString(urlStr));
    QNetworkRequest request(url);

    QNetworkReply *reply = m_manager->get(request);

    connect(reply, &QNetworkReply::finished, this, &HttpClient::handleReply);
}

void HttpClient::sendPostRequest(const std::string &urlStr, const std::string &postDataStr)
{
    QUrl url = QUrl::fromUserInput(QString::fromStdString(urlStr));
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QByteArray postData = QByteArray::fromStdString(postDataStr);

    QNetworkReply *reply = m_manager->post(request, postData);

    connect(reply, &QNetworkReply::finished, this, &HttpClient::handleReply);
}

void HttpClient::handleReply()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (!reply)
        return;

    if (reply->error() == QNetworkReply::NoError)
    {
        QByteArray response = reply->readAll();
        std::string resString(response.constData(), response.size());
        emit responseReceived(resString);
    }
    else
    {
        emit responseReceived("");
    }
    reply->deleteLater();
}
