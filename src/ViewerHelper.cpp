#include "ViewerHelper.h"

namespace ViewerHelper {

float computeScale(QSize src, QSize dst, DrawMode mode)
{
    switch (mode) {
    case DrawMode::AspectFill:
        return std::max(
                    dst.width() / float(src.width()),
                    dst.height() / float(src.height())
                    );
    case DrawMode::AspectFit:
        return std::min(
                    dst.width() / float(src.width()),
                    dst.height() / float(src.height())
                    );
    }
    return 1.0f;

}

#ifndef Q_OS_MAC
void ViewerHelper::disablePlatformSpecificMenuItems()
{
}
#endif // Q_OS_MAC

}
