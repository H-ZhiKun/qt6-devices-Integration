#pragma once
#include <QImage>
#include <QQuickItem>
#include <QQuickWindow>
#include <QSGNode>
#include <QSGSimpleRectNode>
#include <QSGSimpleTextureNode>

class ImagePainter : public QQuickItem
{
    Q_OBJECT
  public:
    explicit ImagePainter(QQuickItem *parent = nullptr);
    virtual ~ImagePainter()
    {
    }

  public slots:
    void updateImage(const QImage &);
  public slots:
    void stopPainting();

  protected:
    QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *) override;

  private:
    QImage imgCache_;
    QImage initImg_;
};