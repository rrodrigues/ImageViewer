#ifndef IMAGE_VIEWER_H
#define IMAGE_VIEWER_H

#include <QWidget>
#include "ViewerHelper.h"

class Viewer;

class QLabel;
class QAction;

class ImageViewer : public QWidget
{
public:
    ImageViewer(QWidget* parent = nullptr);

    void openFileDialog();
    void openFile(QString filename);
    void showImage(QImage image, QString info = "<buffer>");

    void setMode(DrawMode m);

    QSize sizeHint() const override;

    void restoreWindowState();
    void saveWindowState() const;

protected:
    void showEvent(QShowEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent * event) override;

private:
    void createActions();
    void createMenus();
    void toogleTools();
    void toogleInfo();

    Viewer* mViewer;
    QWidget* mToolsWidget;
    QLabel* mInfo;

    QAction* mOpenAction;

    QAction* mZoomInAction;
    QAction* mZoomOutAction;
    QAction* mResetZoomAction;
    QAction* mZoomtoFitAction;

    QAction* mToogleToolsAction;
    QAction* mToogleInfoAction;
};


#endif  // IMAGE_VIEWER_H
