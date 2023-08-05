#include "HttpApiManager.h"
#include "Logger.h"
#include "json/json.h"
HttpApiManager::HttpApiManager(QObject *parent) : QObject(parent)
{
}

void HttpApiManager::post(const QString &url, const QString &data, cv::Mat &matImage)
{
    QUrl qurl(url);
    QNetworkRequest request(qurl);

    // 设置请求头
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QNetworkReply *reply = m_networkManager.post(request, data.toUtf8());
    connect(reply, &QNetworkReply::finished, this,
            [this, reply, matImage]() { onReplyFinished(reply, matImage.clone()); });
}

void HttpApiManager::onReplyFinished(QNetworkReply *reply, cv::Mat matImage)
{
    if (reply->error() != QNetworkReply::NoError)
    {
        qDebug() << "NetWork Eror: " << reply->errorString();
        LogError("NetWork Eror");
        reply->deleteLater();
        emit requestFinished(QString(), matImage.clone()); // 或者执行适当的错误处理
        return;
    }

    QByteArray responseData = reply->readAll();
    reply->deleteLater();
    emit requestFinished(QString(responseData), matImage.clone());
}