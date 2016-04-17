#-------------------------------------------------
#
# Project created by QtCreator 2016-04-06T20:14:24
#
#-------------------------------------------------
include (qtsoap.pri)
QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = soaptest
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qextserialbase.cpp \
    posix_qextserialport.cpp \
    uploadthread.cpp \
    inputthread.cpp

HEADERS  += mainwindow.h \
    posix_qextserialport.h \
    qextserialbase.h \
    uploadthread.h \
    inputthread.h

FORMS    += mainwindow.ui

OTHER_FILES +=
