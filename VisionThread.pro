QT       += core gui

TARGET = VisionThread
TEMPLATE = app


SOURCES += main.cpp\
        Display.cpp \
    ThreadWaiter.cpp \
    VideoCapture.cpp

HEADERS  += Display.h \
    ThreadWaiter.h \
    VideoCapture.h

FORMS    += Display.ui

# video input
INCLUDEPATH += $$quote(c:/home/artm/src/videoInput)
LIBS += -L$$quote(c:/home/artm/src/build/videoInput-mingw) -lvideoInput
LIBS += -luuid -lstrmiids -lole32 -loleaut32

OTHER_FILES += \
    Readme.md
