QT       += core gui

TARGET = VisionThread
TEMPLATE = app


SOURCES += main.cpp\
        Display.cpp \
    VideoThread.cpp

HEADERS  += Display.h \
    VideoThread.h

FORMS    += Display.ui

# video input
INCLUDEPATH += $$quote(c:/home/artm/src/videoInput)
LIBS += -L$$quote(c:/home/artm/src/build/videoInput-mingw) -lvideoInput
LIBS += -luuid -lstrmiids -lole32 -loleaut32
