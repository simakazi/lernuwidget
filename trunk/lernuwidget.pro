TEMPLATE = app
LANGUAGE = C++
TARGET = lernuwidget
DESTDIR = ./bin
QT += core gui network
CONFIG += release
DEFINES += QT_NETWORK_LIB
INCLUDEPATH += ./tmp \
    .
DEPENDPATH += .
MOC_DIR += ./tmp
OBJECTS_DIR += ./tmp
UI_DIR += ./tmp
RCC_DIR += ./tmp

include(lernuwidget.pri)
