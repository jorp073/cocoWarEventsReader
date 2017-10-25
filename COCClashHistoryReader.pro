#-------------------------------------------------
#
# Project created by QtCreator 2017-10-25T00:03:30
#
#-------------------------------------------------

QT       += core gui
QT       += testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = COCClashHistoryReader
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


INCLUDEPATH += F:\opencv3.3.1\build\include
LIBS     += -L"F:\opencv3.3.1\build\lib\Debug" \
    -lopencv_core331d -lopencv_highgui331d -lopencv_imgcodecs331d \
    -lopencv_imgproc331d

SOURCES += main.cpp\
        mainwindow.cpp \
    recognizer/dialogrecognizer.cpp \
    test/test.cpp

HEADERS  += mainwindow.h \
    recognizer/dialogrecognizer.h \
    test/test.h

FORMS    += mainwindow.ui
