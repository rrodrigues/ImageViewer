#ifndef VIEWER_HELPER_H
#define VIEWER_HELPER_H

#include <QSize>

enum class DrawMode {
    FixToWindow,
    AspectFit,
    AspectFill,
};

namespace ViewerHelper {

float computeScale(QSize src, QSize dst, DrawMode mode);

void disablePlatformSpecificMenuItems();

}

#endif // VIEWER_HELPER_H
