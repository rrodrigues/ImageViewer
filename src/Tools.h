#ifndef TOOLS_H
#define TOOLS_H

#include <QRectF>
#include <QCursor>

class Viewer;

class QMouseEvent;
class QPainter;
class QKeyEvent;

class Tool
{
public:
    void setViewer(Viewer* viewer);

    virtual QCursor cursor() const { return QCursor(); }

    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);

    virtual void paintEvent(QPainter* pPainter, QRectF drawRect, float scale) const;

    virtual void keyPressEvent(QKeyEvent *event) const;

protected:
    QPointF getDeltaMoviment(QMouseEvent *event);

    Viewer* mViewer;
    QRectF mRect;

};

namespace Tools {

    Tool* move();
    Tool* selection();

}

#endif // TOOLS_H
