QT += core
QT += gui
QT += widgets

CONFIG += c++11

TARGET = MVews
#CONFIG += console
#CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    titlebartest.cpp \
    virtualframe.cpp \
    actionmanager.cpp \
    resizermaf.cpp

FORMS += \
    titlebartest.ui

HEADERS += \
    titlebartest.h \
    virtualframe.h \
    actionmanager.h \
    resizermaf.h

RESOURCES +=
