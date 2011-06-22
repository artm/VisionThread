#include <videoInput.h>

#include "VideoThread.h"

#include <QDebug>

VideoThread::VideoThread(QObject *parent)
    : QThread(parent)
    , m_cams(new videoInput)
    , m_openCam(-1)
{
    Q_ASSERT(connect(&m_clock, SIGNAL(timeout()), SLOT(poll())));
}

VideoThread::~VideoThread()
{
    if (m_openCam > -1)
        m_cams->stopDevice(m_openCam);
    delete m_cams;
}

void VideoThread::run()
{
    Q_ASSERT(m_cams);
    qDebug() << "starting the video thread's main loop";
    int nCams = m_cams->listDevices();
    QStringList camNames;
    for(int i = 0; i < nCams; ++i)
        camNames << m_cams->getDeviceName(i);
    emit foundCameras(camNames);
    qDebug() << "Found video devices" << camNames;

    exec();
}

void VideoThread::openCamera(int index)
{
    if (m_openCam > -1)
        m_cams->stopDevice(m_openCam);

    m_openCam = index;

    if (m_openCam > -1) {
        qDebug() << "opened camera" << m_openCam;
        m_cams->setIdealFramerate(m_openCam, 25);
        m_cams->setAutoReconnectOnFreeze(m_openCam,true,7);

        /* select resolution?
        if (ui->resolution->currentIndex()) {
            QStringList strs = ui->resolution->currentText().split('x');
            Q_ASSERT(strs.length() == 2);
            m_cams->setupDevice(m_openCam, strs[0].toInt(), strs[1].toInt());
        } else
        */
        m_cams->setupDevice(m_openCam,320,240); // force resolution for iSight in Parallels
        m_clock.start(40);
    }
}

void VideoThread::poll()
{
    if ((m_openCam > -1) && m_cams->isFrameNew(m_openCam)) {
        QImage frame  = QImage(m_cams->getWidth(m_openCam),
                               m_cams->getHeight(m_openCam),
                               QImage::Format_RGB888);
        m_cams->getPixels(m_openCam, frame.bits(), true, true);
        emit gotFrame(frame);
    }
}

