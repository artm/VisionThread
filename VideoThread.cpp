#include <videoInput.h>

#include "VideoThread.h"

#include <QDebug>
#include <QApplication>

VideoThread::VideoThread(QObject *parent)
    : QThread(parent)
    , m_cams(0)
    , m_openCam(-1)
    , m_resW(0)
    , m_resH(0)
{

}

VideoThread::~VideoThread()
{
    if (m_cams) {
        if (m_openCam > -1)
            m_cams->stopDevice(m_openCam);
        delete m_cams;
    }
}

void VideoThread::run()
{
    m_cams = new videoInput;
    Q_ASSERT(m_cams);
    searchCameras();

    Q_ASSERT(qApp);
    Q_ASSERT(connect(qApp, SIGNAL(aboutToQuit()), SLOT(shutdown()), Qt::DirectConnection));
    Q_ASSERT(connect(&m_clock, SIGNAL(timeout()), SLOT(poll())));

    exec();
}

void VideoThread::openCamera(int index)
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

        /* select resolution?
        if (ui->resolution->currentIndex()) {
            QStringList strs = ui->resolution->currentText().split('x');
            Q_ASSERT(strs.length() == 2);
            m_cams->setupDevice(m_openCam, strs[0].toInt(), strs[1].toInt());
        } else
        */
        if (m_resW && m_resH)
            m_cams->setupDevice(m_openCam,m_resW,m_resH);
        else {
            m_cams->setupDevice(m_openCam);
            // let the world know what windows has chosen for us...
            emit autoResolution( m_cams->getWidth(m_openCam), m_cams->getHeight(m_openCam) );
        }
        m_clock.start(40);
    }
}

void VideoThread::poll()
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

void VideoThread::searchCameras()
{
    Q_ASSERT(m_cams);

    // stupid, that videoInput doesn't do that itself, but...
    int nCams = m_cams->devicesFound = m_cams->listDevices();
    QStringList camNames;
    for(int i = 0; i < nCams; ++i)
        camNames << m_cams->getDeviceName(i);
    emit foundCameras(camNames);
}

void VideoThread::closeCamera()
{
    Q_ASSERT(m_cams);

    m_clock.stop();
    if (m_openCam > -1)
        m_cams->stopDevice(m_openCam);
}

void VideoThread::shutdown()
{
    exit();
    wait(2000);
}

void VideoThread::setupResolution(int w, int h)
{
    m_resW = w;
    m_resH = h;

    // if camera is open...
    if (m_cams && m_openCam > -1) {
        // ... reopen with new settings
        openCamera(m_openCam);
    }
}

