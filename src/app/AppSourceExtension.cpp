#include "AppSourceExtension.h"
#include "Logger.h"
#include "Utils.h"
#include <QDir>

void AppFrame::AppSourceExtension::initFilePath(QString &&appPath)
{
    QDir qdir;
    pathNormalImage_ = appPath + "/Image/normal/";
    if (!qdir.mkpath(pathNormalImage_))
    {
        LogWarn("create dir error: {}", pathNormalImage_.toStdString());
    }
    pathNGImage_ = appPath + "/Image/NG/";
    if (!qdir.mkpath(pathNGImage_))
    {
        LogWarn("create dir error: {}", pathNGImage_.toStdString());
    }
}

QString AppFrame::AppSourceExtension::getFilePath(const ResourcePath path, const QString &idNumber)
{
    QString retPath;
    switch (path)
    {
    case AppFrame::ResourcePath::PathImgNormal: {
        retPath = pathNormalImage_ + QDateTime::currentDateTime().toString("yyyy_MM_dd") + "/" +
                  QDateTime::currentDateTime().toString("hh_mm_ss") + "_" + idNumber + "/";
        break;
    }
    case AppFrame::ResourcePath::PathImgNG: {
        retPath = pathNGImage_ + QDateTime::currentDateTime().toString("yyyy_MM_dd") + "/" +
                  QDateTime::currentDateTime().toString("hh_mm_ss") + "_" + idNumber + "/";
        break;
    }
    default:
        break;
    }
    QDir qdir;
    if (!qdir.mkpath(retPath))
    {
        LogError("create dir error: {}", retPath.toStdString());
    }
    return retPath;
}

void AppFrame::AppSourceExtension::deleteImage(const uint16_t &mouthNormal, const uint16_t &mouthNG, QString &&appPath)
{
    Utils::asyncTask([mouthNormal, mouthNG, appPath] {
        QDir folderNormal(appPath + pathNormalImage_);
        QDir folderNG(appPath + pathNGImage_);
        QFileInfoList fileNormalInfoList = folderNormal.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
        QFileInfoList fileNGInfoList = folderNG.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
        foreach (const QFileInfo &fileInfo, fileNormalInfoList)
        {
            if (fileInfo.isDir())
            {
                QDateTime folderCreationTime = fileInfo.birthTime();
                if (folderCreationTime.addMonths(mouthNormal).date() < QDateTime::currentDateTime().date())
                {
                    QDir folder_(fileInfo.absoluteFilePath());
                    folder_.removeRecursively();
                }
            }
        }
        foreach (const QFileInfo &fileInfo, fileNGInfoList)
        {
            if (fileInfo.isDir())
            {
                QDateTime folderCreationTime = fileInfo.birthTime();
                if (folderCreationTime.addMonths(mouthNG).date() < QDateTime::currentDateTime().date())
                {
                    QDir folder_(fileInfo.absoluteFilePath());
                    folder_.removeRecursively();
                }
            }
        }
    });
}
