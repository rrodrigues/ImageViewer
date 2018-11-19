
#include <QApplication>
#include <QFileInfo>
#include "ImageViewer.h"
#include "opencv2/highgui_ex.h"

void testViewer(int argv, char** argc)
{
    QApplication app(argv, argc);
    app.setQuitOnLastWindowClosed(false);

    ImageViewer v;
    v.show();

    app.exec();
}

void testOpencv(int argv, char** argc)
{
    for (int i = 1 ; i < argv; ++i) {
        QFileInfo fi(argc[i]);
        cv::Mat image = cv::imread(argc[i]);

        std::string name = fi.baseName().toStdString();
        cv::ex::imshow(name, image);

        using namespace std::chrono_literals;
        cv::ex::waitKey(2s);
    }

    cv::ex::waitKey();
}

int main(int argv, char** argc) {

//    testViewer(argv, argc);

    testOpencv(argv, argc);
}
