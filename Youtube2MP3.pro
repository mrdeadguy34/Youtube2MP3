#-------------------------------------------------
#
# Project created by QtCreator 2018-08-13T21:31:13
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Youtube2MP3
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        basewindow.cpp \
        datalabel.cpp \
    filedownload.cpp

HEADERS += \
        basewindow.h \
        datalabel.h \
        filedownload.h

RC_FILE += Bin/y2mp3.rc

#DISTFILES += \
#    Bin/JSONLoad.py \
#    Bin/ThumbNailLoad.py

#LIBS += -L$${PWD}/Libs
LIBS += $${PWD}/Libs/libcurl.dll.a

INCLUDEPATH += $${PWD}/Libs/includes
