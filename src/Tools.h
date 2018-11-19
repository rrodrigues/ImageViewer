#ifndef TOOLS_H
#define TOOLS_H

#include <QRectF>
#include <QCursor>

class QMouseEvent;
class QPainter;
class QKeyEvent;

class Tool
{
public:
    virtual QCursor cursor() const { return QCursor(); }

    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual QPointF mouseMoveEvent(QMouseEvent *event);

    virtual void paintEvent(QPainter* pPainter, QRectF drawRect, float scale) const;

    virtual void keyPressEvent(QKeyEvent *event) const;

    QRectF mRect;
};

namespace Tools {

    Tool* move();
    Tool* selection();

}

#endif // TOOLS_H
