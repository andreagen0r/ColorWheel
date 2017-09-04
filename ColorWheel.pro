QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ColorWheel
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
        main.cpp \
        colorwheel.cpp \
        form.cpp \
        phvector3.cpp \
        phmath.cpp

HEADERS += \
        colorwheel.h \
        form.h \
        phvector3.h \
        phmath.h

FORMS += \
        form.ui

QMAKE_CXXFLAGS += -msse4.1

#QMAKE_CXXFLAGS_RELEASE -= -O3
