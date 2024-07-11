#include "AppUdpCameraExtension.h"
#include "AppMetaFlash.h"
#include "AppSourceExtension.h"
#include "BasePLCDevice.h"
#include "Logger.h"
#include <qimage.h>

void AppFrame::AppUdpCameraExtension::aftercameraTragger(const QString windName, QByteArray &&bMat)
{
    appFramework().runAfter("aftercameraTragger", [windName, bMat] {
        const auto &plc = appFramework().getPlugin<BasePLCDevice>();
        const auto *meta = qobject_cast<AppMetaFlash *>(appFramework().getMeta());
        std::string windStr = meta->getWindName(windName.mid(7).toStdString());
        if (!plc)
        {
            LogInfo("windStr not found");
            return;
        }
        cv::Mat matImage(1080, 1920, CV_8UC1);
        // Copy QByteArray data to cv::Mat
        uchar *matData = matImage.data;
        memcpy(matData, bMat.constData(), bMat.size());
        const auto &engine = appFramework().getEngine();
        if (engine == nullptr)
            return;
        // newImage = mat;
        // productKey = ProductItemKey::ocr_path;
        // auto currentProduct = product->updateValue(0, ProductItemKey::ocr_path, ProductImgaeData{"1", mat});
        const auto PathNG = AppSourceExtension::getFilePath(ResourcePath::PathImgNG).toStdString() +
                            Utils::getCurrentTime(true) + ".jpg";
        Utils::saveMatToLocal(matImage, PathNG);
        QImage img = Utils::matToQImage(matImage).scaled(270, 270, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        appFramework().invokeCpp(engine->rootObjects().first()->findChild<QObject *>(windStr.c_str()), "updateImage",
                                 img);
    });
}