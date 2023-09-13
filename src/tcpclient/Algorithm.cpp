#include "Algorithm.h"

Algorithm::Algorithm(QObject *parent)
{
}

Algorithm::~Algorithm()
{
}

void Algorithm::dealing(std::vector<unsigned char> buffer)
{
    std::string str(buffer.begin(), buffer.end());
    emit responseReceived(str);
}

void Algorithm::pingBehavior()
{
}

void Algorithm::sendJsonData(const std::string &jsonData)
{
    sendData(QByteArray(reinterpret_cast<const char *>(jsonData.data()), static_cast<int>(jsonData.size())));
    qDebug() << "http jsonData length   " << jsonData.length();
}
