QT       += core gui

TARGET = VisionThread
TEMPLATE = app


SOURCES += main.cpp\
        Display.cpp \
    ThreadWaiter.cpp \
    VideoCapture.cpp \
    VideoCaptureControls.cpp \
    FaceTracker.cpp

HEADERS  += Display.h \
    ThreadWaiter.h \
    VideoCapture.h \
    VideoCaptureControls.h \
    FaceTracker.h

FORMS    += Display.ui

# video input
INCLUDEPATH += $$quote(c:/home/artm/src/videoInput)
LIBS += -L$$quote(c:/home/artm/src/build/videoInput-mingw) -lvideoInput
LIBS += -luuid -lstrmiids -lole32 -loleaut32

# OpenCV (what about OpenCV's own cmake files?)
INCLUDEPATH += $$quote(c:/home/artm/src/sdk/OpenCV-2.2.0/include/opencv)
INCLUDEPATH += $$quote(c:/home/artm/src/sdk/OpenCV-2.2.0/modules/calib3d/include)
INCLUDEPATH += $$quote(c:/home/artm/src/sdk/OpenCV-2.2.0/modules/contrib/include)
INCLUDEPATH += $$quote(c:/home/artm/src/sdk/OpenCV-2.2.0/modules/core/include)
INCLUDEPATH += $$quote(c:/home/artm/src/sdk/OpenCV-2.2.0/modules/features2d/include)
INCLUDEPATH += $$quote(c:/home/artm/src/sdk/OpenCV-2.2.0/modules/flann/include)
INCLUDEPATH += $$quote(c:/home/artm/src/sdk/OpenCV-2.2.0/modules/gpu/include)
INCLUDEPATH += $$quote(c:/home/artm/src/sdk/OpenCV-2.2.0/modules/highgui/include)
INCLUDEPATH += $$quote(c:/home/artm/src/sdk/OpenCV-2.2.0/modules/imgproc/include)
INCLUDEPATH += $$quote(c:/home/artm/src/sdk/OpenCV-2.2.0/modules/legacy/include)
INCLUDEPATH += $$quote(c:/home/artm/src/sdk/OpenCV-2.2.0/modules/ml/include)
INCLUDEPATH += $$quote(c:/home/artm/src/sdk/OpenCV-2.2.0/modules/objdetect/include)
INCLUDEPATH += $$quote(c:/home/artm/src/sdk/OpenCV-2.2.0/modules/video/include)
LIBS += -Lc:/home/artm/src/sdk/OpenCV-build-win32/3rdparty/lib
LIBS += -Lc:/home/artm/src/sdk/OpenCV-build-win32/lib
# on windows opencv libraries have 220 (version number) attached to filename :(
#LIBS += -lopencv_calib3d220
#LIBS += -lopencv_contrib220
#LIBS += -lopencv_features2d220
#LIBS += -lopencv_flann220
#LIBS += -lopencv_ffmpeg220
#LIBS += -lopencv_gpu220
#LIBS += -lopencv_haartraining_engine
#LIBS += -lopencv_highgui220
#LIBS += -lopencv_legacy220
#LIBS += -lopencv_ml220
LIBS += -lopencv_video220
LIBS += -lopencv_objdetect220
LIBS += -lopencv_imgproc220
LIBS += -lopencv_core220
LIBS += -llibjasper -llibjpeg -llibpng -lopencv_lapack -lzlib

OTHER_FILES += \
    Readme.md
