#include "HttpClient.h"
#include <QHttpMultiPart>

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
    LogInfo("send data to {}", urlStr);
    QByteArray postData = QByteArray::fromStdString(postDataStr);

    QNetworkReply *reply = m_manager->post(request, postData);

    connect(reply, &QNetworkReply::finished, this, &HttpClient::handleReply);
}

void HttpClient::sendImageRequest(const std::string &urlStr, const QByteArray &imageData, const std::string &imageName,
                                  const std::string &jsonText)
{
    QUrl url = QUrl::fromUserInput(QString::fromStdString(urlStr));
    QNetworkRequest request(url);

    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    // Add image data part
    QHttpPart imagePart;
    imagePart.setHeader(QNetworkRequest::ContentDispositionHeader,
                        QVariant("form-data; name=\"image\"; filename=\"" + QString::fromStdString(imageName) + "\""));
    imagePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("image/jpeg"));
    imagePart.setBody(imageData);
    multiPart->append(imagePart);

    // Add JSON part
    QHttpPart jsonPart;
    jsonPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"json\""));
    jsonPart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));
    jsonPart.setBody(jsonText.c_str());
    multiPart->append(jsonPart);

    QNetworkReply *reply = m_manager->post(request, multiPart);

    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    // 下面处理异常
    // connect(reply, &QNetworkReply::finished, this, &HttpClient::handleReply);
    connect(reply, &QNetworkReply::finished, multiPart, &QHttpMultiPart::deleteLater);
    loop.exec(); // 阻塞，直到回复完成

    // 在这里可以处理 reply 的结果，以及其它你需要的操作
    // reply->error() 可以检查是否有错误发生
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

    reply->deleteLater(); // 清理 reply 对象
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
