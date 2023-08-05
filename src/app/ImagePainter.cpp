#include "ImagePainter.h"
#include <QGuiApplication>

ImagePainter::ImagePainter(QQuickItem *parent) : QQuickItem(parent)
{
    // 这句不加会报错
    setFlag(ItemHasContents, true);
    // 默认图片不可以空，否则渲染引擎会出现异常，造成最开始的帧显示错误
    QString strPath = qApp->applicationDirPath();
    imgCache_ = QImage(strPath + "/ico/novideo.jpg");
    initImg_ = imgCache_;
}

void ImagePainter::updateImage(const QImage &image)
{
    imgCache_ = image; // 创建新的 QImage 对象
    update();
}
void ImagePainter::stopPainting()
{
    imgCache_ = initImg_;
    update();
}

QSGNode *ImagePainter::updatePaintNode(QSGNode *oldNode, QQuickItem::UpdatePaintNodeData *)
{
    auto node = dynamic_cast<QSGSimpleTextureNode *>(oldNode);

    if (!node)
    {
        node = new QSGSimpleTextureNode();
    }

    QSGTexture *m_texture = window()->createTextureFromImage(imgCache_, QQuickWindow::TextureIsOpaque);
    node->setOwnsTexture(true);
    node->setRect(boundingRect());
    node->markDirty(QSGNode::DirtyForceUpdate);
    node->setTexture(m_texture);

    return node;
}