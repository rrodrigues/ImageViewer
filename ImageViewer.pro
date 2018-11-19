#-------------------------------------------------
#
# Project created by QtCreator 2018-11-15T22:45:14
#
#-------------------------------------------------

QT += core gui
QT += widgets

TARGET = ImageViewer
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    main.cpp


include(src/ImageViewer.pri)
include(integrations/opencv.pri)
