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
    actionmanager.cpp

FORMS += \
    titlebartest.ui

HEADERS += \
    titlebartest.h \
    virtualframe.h \
    actionmanager.h

RESOURCES += \
    rest.qrc
