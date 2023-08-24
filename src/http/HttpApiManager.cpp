#include "HttpApiManager.h"
#include "Logger.h"
#include "json/json.h"
HttpApiManager::HttpApiManager(QObject *parent) : QObject(parent)
{
}

void HttpApiManager::post(const QString &url, const QString &data, cv::Mat &matImage, const int bottomNum)
{
    QNetworkAccessManager m_networkManager;
    QUrl qurl(url);
    QNetworkRequest request(qurl);

    // 设置请求头
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QNetworkReply *reply = m_networkManager.post(request, data.toUtf8());
    connect(reply, &QNetworkReply::finished, this,
            [this, reply, matImage, bottomNum]() { onReplyFinished(reply, matImage.clone(), bottomNum); });
}

void HttpApiManager::onReplyFinished(QNetworkReply *reply, cv::Mat matImage, const int bottomNum)
{
    if (reply->error() != QNetworkReply::NoError)
    {
        qDebug() << "NetWork Eror: " << reply->errorString();
        LogError("NetWork Eror", reply->errorString().toStdString());
        reply->deleteLater();
        emit requestFinished(QString(), matImage.clone(), bottomNum); // 或者执行适当的错误处理
        return;
    }

    QByteArray responseData = reply->readAll();
    reply->deleteLater();
    emit requestFinished(QString(responseData), matImage.clone(), bottomNum);
}