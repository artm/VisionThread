#include <videoInput.h>

#include "VideoCapture.h"

#include <QDebug>
#include <QApplication>

VideoCapture::VideoCapture(QThread * thread, QObject *parent)
    : QObject(parent)
    , m_cams(0)
    , m_openCam(-1)
    , m_resW(0)
    , m_resH(0)
    , m_clock(0)
{
    moveToThread(thread);
    Q_ASSERT(connect(thread, SIGNAL(started()), SLOT(onThreadStarted())));
}

VideoCapture::~VideoCapture()
{
    qDebug() << "Shutting down video capture";
    if (m_cams)
        closeCamera();
}

void VideoCapture::onThreadStarted()
{
    qDebug() << "Video thread started, initialize capture";
    m_clock = new QTimer(this);
    Q_ASSERT(m_clock);
    Q_ASSERT(connect(m_clock, SIGNAL(timeout()), SLOT(onClockTick())));

    m_cams = new videoInput;
    Q_ASSERT(m_cams);
    scanCameras();
}

void VideoCapture::openCamera(int index)
{
    Q_ASSERT(m_cams);
    if (index < -1 || index >= m_cams->devicesFound) {
        qCritical() << "Camera index out of range:" << index;
        return;
    }

    closeCamera();

    m_openCam = index;
    if (m_openCam > -1) {
        m_cams->setIdealFramerate(m_openCam, 25);
        m_cams->setAutoReconnectOnFreeze(m_openCam,true,7);

        if (m_resW && m_resH)
            m_cams->setupDevice(m_openCam,m_resW,m_resH);
        else {
            m_cams->setupDevice(m_openCam);
            // let the world know what windows has chosen for us...
            emit autoResolution( m_cams->getWidth(m_openCam), m_cams->getHeight(m_openCam) );
        }
        m_clock->start(40);
    }
}

void VideoCapture::onClockTick()
{
    Q_ASSERT(m_cams);

    if ((m_openCam > -1) && m_cams->isFrameNew(m_openCam)) {
        QImage frame  = QImage(m_cams->getWidth(m_openCam),
                               m_cams->getHeight(m_openCam),
                               QImage::Format_RGB888);
        m_cams->getPixels(m_openCam, frame.bits(), true, true);
        emit gotFrame(frame);
    }
}

void VideoCapture::scanCameras()
{
    Q_ASSERT(m_cams);

    // stupid, that videoInput doesn't do that itself, but...
    int nCams = m_cams->devicesFound = m_cams->listDevices();
    QStringList camNames;
    for(int i = 0; i < nCams; ++i)
        camNames << m_cams->getDeviceName(i);
    emit foundCameras(camNames);
}

void VideoCapture::closeCamera()
{
    Q_ASSERT(m_cams);

    if (m_openCam > -1)
        m_cams->stopDevice(m_openCam);
}

void VideoCapture::setupResolution(int w, int h)
{
    m_resW = w;
    m_resH = h;

    // if camera is open...
    if (m_cams && m_openCam > -1) {
        // ... reopen with new settings
        openCamera(m_openCam);
    }
}

