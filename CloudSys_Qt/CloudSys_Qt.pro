#-------------------------------------------------
#
# Project created by QtCreator 2015-08-15T12:05:03
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QMAKE_CXXFLAGS += -std=c++0x

TARGET = CloudSys_Qt
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    login.cpp \
    filehandler.cpp \
    global.cpp \
    iconprovider.cpp \
    icondisplayerwidget.cpp \
    fileinforesolver.cpp \
    tcp.cpp \
    ftpclient.cpp \
    dokanthread.cpp

HEADERS  += mainwindow.h \
    login.h \
    filehandler.h \
    global.h \
    iconprovider.h \
    icondisplayerwidget.h \
    fileinforesolver.h \
    tcp.h \
    ftpclient.h \
    dokanthread.h

FORMS    += mainwindow.ui \
    login.ui \
    icondisplayerwidget.ui
QT += network
