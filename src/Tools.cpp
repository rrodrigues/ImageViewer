#include "Tools.h"

#include "Viewer.h"

#include <QMouseEvent>
#include <QDebug>
#include <QPainter>
#include <QClipboard>
#include <QApplication>

class MoveTool : public Tool
{
public:
    QCursor cursor() const override { return QCursor(Qt::ClosedHandCursor); }

    void mouseMoveEvent(QMouseEvent *event) override;
};

class SelectionTool : public Tool
{
public:
    QCursor cursor() const override { return QCursor(Qt::CrossCursor); }

    void paintEvent(QPainter* pPainter, QRectF drawRect, float scale) const override;

    void keyPressEvent(QKeyEvent *event) const override;
};

void Tool::setViewer(Viewer *viewer)
{
    mViewer = viewer;
}

void Tool::mousePressEvent(QMouseEvent* event)
{
    mRect.setTopLeft(event->localPos());
    mRect.setBottomRight(event->localPos());
}

void Tool::mouseReleaseEvent(QMouseEvent*)
{
    mRect.setRect(0, 0, 0, 0);
}

void Tool::mouseMoveEvent(QMouseEvent* event)
{
    mRect.setBottomRight(event->localPos());
}

void Tool::paintEvent(QPainter* , QRectF , float ) const
{
}

void Tool::keyPressEvent(QKeyEvent *) const
{
}

QPointF Tool::getDeltaMoviment(QMouseEvent* event)
{
    QPointF tl = mRect.bottomRight();
    Tool::mouseMoveEvent(event);
    mRect.setTopLeft(tl);
    return QPointF(mRect.width(), mRect.height());
}

void MoveTool::mouseMoveEvent(QMouseEvent *event)
{
    QPointF delta = getDeltaMoviment(event);
    if (mViewer) {
        mViewer->translate(delta);
    }
}

void SelectionTool::paintEvent(QPainter *pPainter, QRectF drawRect, float scale) const
{
    if (pPainter == nullptr) {
        return;
    }
    QPainter& painter = *pPainter;

    const QRect rect = mRect.toRect();

    QPointF textPos = rect.bottomRight() + QPointF(10,10);

    QRect scalledRect = mRect.toRect();
    scalledRect.translate(-drawRect.toRect().topLeft());
    scalledRect.setTopLeft(scalledRect.topLeft() / scale);
    scalledRect.setBottomRight(scalledRect.bottomRight() / scale);

    QString text = QString("[%1,%2 - %3x%4] %5%")
            .arg(scalledRect.left())
            .arg(scalledRect.top())
            .arg(scalledRect.width())
            .arg(scalledRect.height())
            .arg(scale * 100,0, 'f', 1);

    const int w = 3;

    painter.setPen(QPen(Qt::GlobalColor::darkGray, w));
    painter.drawRect(rect);
    painter.drawText(textPos, text);

    painter.setPen(QPen(Qt::GlobalColor::white, w - 2));
    painter.drawRect(rect);
    painter.drawText(textPos, text);
}

void SelectionTool::keyPressEvent(QKeyEvent *event) const
{
    if (event->matches(QKeySequence::Copy)) {
        QString text = QString("QRectF(%1, %2, %3, %4)")
                .arg(mRect.x(), 0, 'f', 2)
                .arg(mRect.y(), 0, 'f', 2)
                .arg(mRect.width(), 0, 'f', 2)
                .arg(mRect.height(), 0, 'f', 2);
//        QApplication::clipboard()->setText(text);
        qDebug() << "Nothing copied. Missing scale and offset." << text;
    }
}

namespace Tools {

Tool* move()
{
    static MoveTool tool;
    return &tool;
}

Tool* selection()
{
    static SelectionTool tool;
    return &tool;
}

}
