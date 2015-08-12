#-------------------------------------------------
#
# Project created by QtCreator 2015-07-09T14:23:56
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = Exact_Negative
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    Util.cpp \
    RectObject.cpp \
    ReadFromFile.cpp


INCLUDEPATH +=/usr/include/opencv2


LIBS +=/usr/lib/x86_64-linux-gnu/libopencv_highgui.so.2.4.9
LIBS +=/usr/lib/x86_64-linux-gnu/libopencv_core.so.2.4.9
LIBS +=/usr/lib/x86_64-linux-gnu/libopencv_imgproc.so.2.4.9
