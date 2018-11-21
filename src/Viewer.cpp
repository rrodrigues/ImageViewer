#include "Viewer.h"

#include "Tools.h"

#include <QPainter>
#include <QDebug>

Viewer::Viewer(QWidget* parent) : QOpenGLWidget(parent)
{
    setFocusPolicy(Qt::WheelFocus);
}

void Viewer::setTool(Tool *tool)
{
    if (mTool != nullptr && mTool != tool) {
        mTool->setViewer(nullptr);
    }

    mTool = tool;
    if (mTool) {
        setCursor(mTool->cursor());
        mTool->setViewer(this);
    } else {
        unsetCursor();
    }
    update();
}

void Viewer::setImage(QImage image)
{
    mImage = image;
}

void Viewer::zoomIn()
{
    mZoom *= 1.25f;
    update();
}

void Viewer::zoomOut()
{
    mZoom *= 0.88f;
    update();
}

void Viewer::zoomToFit()
{
    mTranslation.rx() = mTranslation.ry() = 0;
    mZoom = 1.0f;
    update();
}

void Viewer::resetZoom()
{
    mTranslation.rx() = mTranslation.ry() = 0;
    mZoom = 1.0f / ViewerHelper::computeScale(mImage.size(), size(), mMode);
    update();
}

void Viewer::translate(QPointF t)
{
    mTranslation += t;
    update();
}

void Viewer::paintEvent(QPaintEvent * event)
{
    QOpenGLWidget::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), QBrush(Qt::GlobalColor::black));
    if (mImage.isNull()) {
        return;
    }

    float scale = ViewerHelper::computeScale(mImage.size(), size(), mMode) * mZoom;
    QSize drawSize = mImage.size() * scale;

    QPointF translation = mTranslation;

    QPoint corner(
                (size().width() - drawSize.width()) / 2 + translation.x(),
                (size().height() - drawSize.height()) / 2 + translation.y()
                );
    QRect drawRect(corner, drawSize);
    painter.drawImage(drawRect, mImage);

    const bool DEBUG_DRAW = false;
    if (DEBUG_DRAW) {
        qDebug() << "viewerSize" << size()
                 << "imageSize" << mImage.size()
                 << "drawRect" << drawRect << drawRect.isEmpty()
                 << "scale" << scale;
    }

    if (mTool) {
        mTool->paintEvent(&painter, drawRect, scale);
    }
}

void Viewer::mousePressEvent(QMouseEvent * event)
{
    QOpenGLWidget::mousePressEvent(event);
    if (mTool) {
        mTool->mousePressEvent(event);
    }
}

void Viewer::mouseReleaseEvent(QMouseEvent * event)
{
    QOpenGLWidget::mouseReleaseEvent(event);
    if (mTool) {
        mTool->mouseReleaseEvent(event);
    }
}

void Viewer::mouseMoveEvent(QMouseEvent * event)
{
    QOpenGLWidget::mouseMoveEvent(event);
    if (mTool) {
        mTool->mouseMoveEvent(event);
        update();
    }
}

void Viewer::keyPressEvent(QKeyEvent *event)
{
    QOpenGLWidget::keyPressEvent(event);
    if (mTool) {
        mTool->keyPressEvent(event);
    }
}

