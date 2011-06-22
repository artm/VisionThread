QT       += core gui

TARGET = VisionThread
TEMPLATE = app


SOURCES += main.cpp\
        Display.cpp

HEADERS  += Display.h

FORMS    += Display.ui

# video input
INCLUDEPATH += $$quote(~/src/videoInput)
LIBS += -L$$quote(~/src/build/videoInput-mingw) -lvideoInput
LIBS += -luuid -lstrmiids -lole32 -loleaut32
