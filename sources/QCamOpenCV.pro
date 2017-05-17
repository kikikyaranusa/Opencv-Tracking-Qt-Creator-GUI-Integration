#-------------------------------------------------
#
# Project created by QtCreator 2017-05-02T11:38:15
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QCamOpenCV
TEMPLATE = app

INCLUDEPATH += /usr/local/include/opencv
LIBS += -L/usr/local/lib -lopencv_core -lopencv_imgcodecs -lopencv_highgui -lopencv_imgproc -lopencv_videoio -lopencv_tracking

SOURCES += main.cpp\
        mainwindow.cpp \
    qopencvwidget.cpp

HEADERS  += mainwindow.h \
    qopencvwidget.h \
    qopencvlabel.h

FORMS    += \
    mainwindow.ui
