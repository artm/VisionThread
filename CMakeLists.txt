PROJECT(VisionThread)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
FIND_PACKAGE(Qt4 REQUIRED)
FIND_PACKAGE(OpenCV REQUIRED)

SET(VisionThread_HEADERS Display.h ThreadWaiter.h VideoCapture.h
  VideoCaptureControls.h FaceTracker.h)
Set(VisionThread_SOURCES main.cpp Display.cpp ThreadWaiter.cpp
  VideoCapture.cpp VideoCaptureControls.cpp FaceTracker.cpp)
SET(VisionThread_FORMS Display.ui)
#SET(VisionThread_RESOURCES VisionThread.qrc)

QT4_WRAP_CPP(VisionThread_HEADERS_MOC ${VisionThread_HEADERS})
QT4_WRAP_UI(VisionThread_FORMS_HEADERS ${VisionThread_FORMS})
#QT4_ADD_RESOURCES(VisionThread_RESOURCES_RCC ${VisionThread_RESOURCES})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

INCLUDE(${QT_USE_FILE})
ADD_EXECUTABLE(VisionThread ${VisionThread_SOURCES}
  ${VisionThread_HEADERS_MOC}
  ${VisionThread_FORMS_HEADERS})
TARGET_LINK_LIBRARIES(VisionThread ${QT_LIBRARIES} ${OpenCV_LIBS})

