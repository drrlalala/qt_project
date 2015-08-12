#-------------------------------------------------
#
# Project created by QtCreator 2015-07-10T14:29:59
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = Exact_positive
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    ReadFromFile.cpp \
    RectObject.cpp \
    Util.cpp

INCLUDEPATH +=/usr/include/opencv2


LIBS +=/usr/lib/x86_64-linux-gnu/libopencv_highgui.so.2.4.9
LIBS +=/usr/lib/x86_64-linux-gnu/libopencv_core.so.2.4.9
LIBS +=/usr/lib/x86_64-linux-gnu/libopencv_imgproc.so.2.4.9
