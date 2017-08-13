#-------------------------------------------------
#
# Project created by QtCreator 2016-08-23T19:53:30
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = calendar
TEMPLATE = app

TRANSLATIONS += tr_chinese.ts

SOURCES += main.cpp\
        mainwindow.cpp \
    calendarcell.cpp \
    calendarwidget.cpp \
    eventdatabase.cpp \
    eventadder.cpp \
    eventcell.cpp \
    fileviewer.cpp \
    colormanager.cpp \
    global.cpp

HEADERS  += mainwindow.h \
    calendarcell.h \
    calendarwidget.h \
    eventdatabase.h \
    eventadder.h \
    eventcell.h \
    fileviewer.h \
    colormanager.h \
    global.h

FORMS    += mainwindow.ui

RESOURCES += \
    icon.qrc
