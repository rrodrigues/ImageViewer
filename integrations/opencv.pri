

SOURCES += \
    $$PWD/opencv2/highgui_ex.cpp


HEADERS += \
    $$PWD/opencv2/highgui_ex.h

INCLUDEPATH += $$PWD

macx {
    PKG_CONFIG_PATH = $$getenv("PKG_CONFIG_PATH")
    isEmpty(PKG_CONFIG_PATH) {
        error(Missing opencv pkg-config path. Add PKG_CONFIG_PATH envvar or pkg-config path to PATH)
    }
    message("Using opencv pkg-config path: " $$PKG_CONFIG_PATH)

    message("Adding opencv lib dir to rpath: " $$RPATH_OPENCV_DIR)
    RPATH_OPENCV_DIR = $$shell_path($$clean_path($$PKG_CONFIG_PATH/..))

    QT_CONFIG -= no-pkg-config
    CONFIG += link_pkgconfig
    PKGCONFIG += opencv
    QMAKE_RPATHDIR += $$RPATH_OPENCV_DIR
}
