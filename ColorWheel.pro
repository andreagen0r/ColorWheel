QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ColorWheel
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++14

SOURCES += \
        main.cpp \
        colorwheel.cpp \
        form.cpp

HEADERS += \
        colorwheel.h \
        form.h \
    nkVector3.h \
    nkMath.h

FORMS += \
        form.ui

QMAKE_CXXFLAGS += -msse -msse2 -mssse3 -msse4.1 -msse4.2
#QMAKE_CXXFLAGS += $$QMAKE_CFLAGS_AVX2
#QMAKE_CXX -= -march=native
QMAKE_CXX += -O3 -masm=intel -mtune=native


CONFIG(release, debug|release) {
DEFINES += QT_NO_DEBUG_OUTPUT
}

QMAKE_CXXFLAGS_RELEASE += -O3
