QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ColorWheel
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++14

SOURCES += \
        main.cpp \
        colorwheel.cpp \
        form.cpp \
        phvector3.cpp \
        phmath.cpp \
    abstractcolorchooser.cpp

HEADERS += \
        colorwheel.h \
        form.h \
        phvector3.h \
        phmath.h \
    abstractcolorchooser.h

FORMS += \
        form.ui

QMAKE_CXXFLAGS += -msse4 -msse4.1 -msse4.2

CONFIG(release, debug|release) {

DEFINES += QT_NO_DEBUG_OUTPUT
QMAKE_CXXFLAGS_RELEASE -= -O3
}

