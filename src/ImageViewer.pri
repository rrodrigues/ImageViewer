
VER_MAJ = 0
VER_MIN = 1
VER_PAT = 0
VERSION = $${VER_MAJ}.$${VER_MIN}.$${VER_PAT}


CONFIG += c++1z
QMAKE_CXXFLAGS += -std=c++17

SOURCES += \
    $$PWD/ImageViewer.cpp \
    $$PWD/ViewerHelper.cpp \
    $$PWD/Viewer.cpp \
    $$PWD/Tools.cpp

HEADERS += \
    $$PWD/ImageViewer.h \
    $$PWD/ViewerHelper.h \
    $$PWD/Viewer.h \
    $$PWD/Tools.h

INCLUDEPATH += $$PWD

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/widgets/icons
INSTALLS += target

RESOURCES += \
    $$PWD/images.qrc

macx{
    OBJECTIVE_SOURCES += $$PWD/platform/macosx/ViewerHelper_mac.mm
    LIBS += -framework AppKit
}
