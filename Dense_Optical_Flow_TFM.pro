#-------------------------------------------------
#
# Project created by QtCreator 2016-02-06T12:29:39
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Dense_Optical_Flow_TFM
TEMPLATE = app

INCLUDEPATH += /usr/local/include/opencv
LIBS += -L/usr/local/lib -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_video

SOURCES += main.cpp\
        mainwindow.cpp \
    video_player.cpp \
    densetrack.cpp \
    tracker.cpp \
    descriptors.cpp \
    initialize.cpp \
    opticalflow.cpp

HEADERS  += mainwindow.h \
    video_player.h \
    densetrack.h \
    descriptors.h \
    initialize.h \
    opticalflow.h \
    tracker.h

FORMS    += mainwindow.ui
