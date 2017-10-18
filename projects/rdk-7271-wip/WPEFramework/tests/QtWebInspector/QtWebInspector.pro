#-------------------------------------------------
#
# Project created by QtCreator 2013-12-05T03:20:02
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtWebInspector
TEMPLATE = app


SOURCES += main.cpp \
        mainwindow.cpp

HEADERS  += mainwindow.h \
    WebInspectorConnection.h


FORMS    += mainwindow.ui

unix:!macx: LIBS += -L/usr/src/common/QT/Debug -lwebsocket -lgenerics -lwebsockets

INCLUDEPATH += /usr/src/common
DEPENDPATH += /usr/src/common/websocket/thirdparty/linux

