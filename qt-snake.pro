QT          += opengl widgets

HEADERS     = \
              helper.h \
              widget.h \
              window.h \
    scorescreen.h \
    stats.h \
    fruit.h \
    animation.h
SOURCES     = \
              helper.cpp \
              main.cpp \
              widget.cpp \
              window.cpp \
    scorescreen.cpp \
    stats.cpp \
    fruit.cpp \
    animation.cpp
TARGET      = qt-snake

RESOURCES += \
    resources.qrc

QMAKE_CXXFLAGS += -std=c++0x
