#ifdef USE_VIDEO_INPUT
#include <videoInput.h>
#else
#include <cv.h>
#include <highgui.h>
#endif

#include "VideoCapture.h"

#include <QDebug>
#include <QApplication>

struct VideoCapture::Private {
#if USE_VIDEO_INPUT
    videoInput * m_cap;
#else
    cv::VideoCapture * m_cap;
#endif
    Private()
        : m_cap(0)
    {}
};

VideoCapture::VideoCapture(QThread * thread, QObject *parent)
    : QObject(parent)
    , m_private(new Private)
    , m_openDevice(-1)
    , m_resW(0)
    , m_resH(0)
    , m_clock(0)
{
    if (!thread) thread = new QThread();
    moveToThread(thread);
    Q_ASSERT(connect(thread, SIGNAL(started()), SLOT(onThreadStarted())));
}

VideoCapture::~VideoCapture()
{
    if (m_private->m_cap)
        closeDevice();
}

void VideoCapture::onThreadStarted()
{
    m_clock = new QTimer(this);
    Q_ASSERT(m_clock);
    Q_ASSERT(connect(m_clock, SIGNAL(timeout()), SLOT(onClockTick())));

#ifdef USE_VIDEO_INPUT
    m_private->m_cap = new videoInput;
#else
    m_private->m_cap = new cv::VideoCapture;
#endif
    Q_ASSERT(m_private->m_cap);
    scanForDevices();
}

void VideoCapture::openDevice(int index)
{
    Q_ASSERT(m_private->m_cap);
    if (index < 0 || index >= m_deviceCount) {
        qCritical() << "Camera index out of range:" << index;
        return;
    }

    closeDevice();

    m_openDevice = index;
#ifdef USE_VIDEO_INPUT
    m_private->m_cap->setIdealFramerate(m_openDevice, 25);
    m_private->m_cap->setAutoReconnectOnFreeze(m_openDevice,true,7);

    if (m_resW && m_resH)
        m_private->m_cap->setupDevice(m_openDevice,m_resW,m_resH);
    else
        m_private->m_cap->setupDevice(m_openDevice);

    m_resW = m_private->m_cap->getWidth(m_openDevice);
    m_resH = m_private->m_cap->getHeight(m_openDevice);
#else
    m_private->m_cap->set(CV_CAP_PROP_FPS, 25);
    if (m_resW && m_resH) {
        m_private->m_cap->set(CV_CAP_PROP_FRAME_WIDTH, m_resW);
        m_private->m_cap->set(CV_CAP_PROP_FRAME_HEIGHT, m_resH);
    }
    if (!m_private->m_cap->open(index)) {
        qCritical() << "Failed to open camera";
        m_openDevice = -1;
        return;
    }
    // properties are just hints - figure them out
    m_resW = (int)m_private->m_cap->get(CV_CAP_PROP_FRAME_WIDTH);
    m_resH = (int)m_private->m_cap->get(CV_CAP_PROP_FRAME_HEIGHT);
#endif
    emit autoResolution( m_resW, m_resH );
    m_clock->start(40);
}

void VideoCapture::onClockTick()
{
    Q_ASSERT(m_private->m_cap);

    QImage frame  = QImage(m_resW, m_resH, QImage::Format_RGB888);

#ifdef USE_VIDEO_INPUT
    if ((m_openDevice > -1) && m_private->m_cap->isFrameNew(m_openDevice)) {
        m_private->m_cap->getPixels(m_openDevice, frame.bits(), true, true);
        emit gotFrame(frame);
    }
#else
    if ((m_openDevice > -1) && m_private->m_cap->grab()) {
        // wrap a cv::Mat around a frame...
        cv::Mat mat;
        m_private->m_cap->retrieve(mat);
        emit gotFrame(frame);
    }
#endif


}

void VideoCapture::scanForDevices()
{
    QStringList camNames;
#ifdef USE_VIDEO_INPUT
    Q_ASSERT(m_private->m_cap);
    // stupid, that videoInput doesn't do that itself, but...
    int nCams = m_private->m_cap->devicesFound = m_private->m_cap->listDevices();
    for(int i = 0; i < nCams; ++i)
        camNames << m_private->m_cap->getDeviceName(i);
#endif
    emit foundDevices(camNames);
}

void VideoCapture::closeDevice()
{
#ifdef USE_VIDEO_INPUT
    Q_ASSERT(m_private->m_cap);
    if (m_openDevice > -1)
        m_private->m_cap->stopDevice(m_openDevice);
#endif
}

void VideoCapture::setupResolution(int w, int h)
{
    m_resW = w;
    m_resH = h;

#ifdef USE_VIDEO_INPUT
    // if camera is open...
    if (m_private->m_cap && m_openDevice > -1) {
        // ... reopen with new settings
        openDevice(m_openDevice);
    }
#endif
}

