#include "highgui_ex.h"

#include "ImageViewer.h"

#include <QMap>
#include <QApplication>
#include <QDebug>
#include <QEvent>
#include <QKeyEvent>

namespace cv {
namespace ex {

class DialogManager : public QEventLoop
{
public:

    static void ensureApplicationExists() {
        if (QApplication::instance() == nullptr) {
            int argc = 0;
            char arg[] = "";
            char* argv[] = {arg};
            new QApplication(argc, argv);
        }
    }

    ImageViewer* getViewer(QString name) {
        auto v = viewers.find(name);
        if (v == viewers.end()) {
            ImageViewer* viewer = new ImageViewer;
            viewer->installEventFilter(this);
            v = viewers.insert(name, viewer);
        }
        return v.value();
    }

    ImageViewer* getViewer(std::string name) {
        return getViewer(QString::fromStdString(name));
    }

    bool eventFilter(QObject *watched, QEvent *event) override {
        if (event->type() == QEvent::KeyRelease) {
            QKeyEvent* keyEvent = dynamic_cast<QKeyEvent*>(event);
            const bool exitEventLoop = keyEvent &&
                    (keyEvent->modifiers() == Qt::NoModifier) &&
                    (ignoreKeys.indexOf(keyEvent->key()) < 0);
            if (exitEventLoop) {
                exitLoop(keyEvent->key());
            }
        } else if (event->type() == QEvent::Close) {
            if (ImageViewer* viewer = dynamic_cast<ImageViewer*>(watched)) {
                QString key = viewers.key(viewer, "");
                if (!key.isEmpty()) {
                    viewers.remove(key);
                }
                if (viewers.empty()) {
                    exitLoop(-1);
                }
            }
        }

        return QEventLoop::eventFilter(watched, event);
    }

    int exec(int ms) {
        if (ms > 0) {
            startTimer(ms);
        } else {

        }
        return QEventLoop::exec();
    }

    void exitLoop(int ret) {
        for (auto v : viewers) {
            if (v->isVisible()) {
                v->saveWindowState();
            }
        }
        exit(ret);
    }

    void destroyAll() {
        for (auto v : viewers) {
            v->close();
            v->deleteLater();
        }
        viewers.clear();
    }

protected:
    void timerEvent(QTimerEvent *event) override {
        QObject::timerEvent(event);
        exit(-1);
        killTimer(event->timerId());
    }

private:
    const QList<int> ignoreKeys = {
        Qt::Key::Key_Shift,
        Qt::Key::Key_Control,
        Qt::Key::Key_Meta,
        Qt::Key::Key_Alt,
        Qt::Key::Key_CapsLock,
        Qt::Key::Key_NumLock,
        Qt::Key::Key_ScrollLock,
        Qt::Key::Key_Super_L,
        Qt::Key::Key_Super_R,
    };
    QMap<QString, ImageViewer*> viewers;    
};


DialogManager& manager() {
    DialogManager::ensureApplicationExists();
    static DialogManager m;
    return m;
}

void namedWindow(const std::string &winname, int /*flags*/)
{
    manager().getViewer(winname)->show();
}

void destroyAllWindows()
{
    manager().destroyAll();
}

void imshow(const std::string &winname, cv::Mat mat)
{
    QImage stub;
    if (!mat.empty()) {
        QImage::Format f;
        if (mat.channels() == 1) {
            f = QImage::Format::Format_Grayscale8;
        } else if (mat.channels() == 3) {
            f = QImage::Format::Format_RGB888;
        } else {
            f = QImage::Format::Format_RGBA8888;
        }

        stub = QImage(mat.cols, mat.rows, f);
        for (int r = 0 ; r < mat.rows; ++r) {
            uchar* src = mat.ptr(r);
            uchar* dst = stub.scanLine(r);
            std::memcpy(dst, src, mat.step);
        }
        stub = stub.rgbSwapped();
    }

    ImageViewer* viewer = manager().getViewer(winname);
    viewer->showImage(stub);
    viewer->setWindowTitle(QString::fromStdString(winname));

    viewer->show();
}

int waitKey(std::chrono::milliseconds ms)
{
    return manager().exec(ms.count());
}

}
}
