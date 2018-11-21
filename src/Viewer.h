#ifndef VIEWER_H
#define VIEWER_H

#include "ViewerHelper.h"

#include <QOpenGLWidget>

class Tool;

class Viewer : public QOpenGLWidget
{
public:
    Viewer(QWidget* parent);

    void setTool(Tool* tool);
    inline Tool* tool() const { return mTool; }

    void setImage(QImage image);
    inline QImage image() const { return mImage; }

    inline void setDrawMode(DrawMode mode) { mMode = mode; }
    inline DrawMode drawMode() const { return mMode; }

    inline void setZoom(float zoom) { mZoom = zoom; }
    inline float zoom() const { return mZoom; }

    void zoomIn();
    void zoomOut();
    void zoomToFit();
    void resetZoom();

    void translate(QPointF t);

protected:
    void paintEvent(QPaintEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

    void keyPressEvent(QKeyEvent *event) override;

private:
    QImage mImage;
    DrawMode mMode = DrawMode::AspectFit;
    float mZoom = 1.0f;
    QPointF mTranslation;

    Tool* mTool = nullptr;
};

#endif // VIEWER_H
