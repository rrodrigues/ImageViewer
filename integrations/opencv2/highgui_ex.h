#ifndef HIGHGUI_EX_H
#define HIGHGUI_EX_H

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

#include <chrono>

namespace cv {
namespace ex {

void namedWindow(const std::string& winname, int flags = cv::WINDOW_AUTOSIZE);

void destroyAllWindows();

void imshow(const std::string& winname, cv::Mat mat);

int waitKey(std::chrono::milliseconds ms = std::chrono::milliseconds(0));

}
}

#endif // HIGHGUI_EX_H
