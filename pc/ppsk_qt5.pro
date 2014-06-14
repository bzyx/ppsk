#-------------------------------------------------
#
# Project created by QtCreator 2014-06-14T14:45:33
#
#-------------------------------------------------

QT       += core gui
CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets serialport

TARGET = ppsk_qt5
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp

HEADERS  += dialog.h

FORMS    += dialog.ui

OTHER_FILES += \
    logo.png

RESOURCES += \
    resources.qrc
