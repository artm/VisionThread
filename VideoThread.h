#ifndef VIDEOTHREAD_H
#define VIDEOTHREAD_H

#include <QThread>
#include <QImage>
#include <QTimer>
#include <QStringList>

class videoInput;

class VideoThread : public QThread
{
    Q_OBJECT
public:
    explicit VideoThread(QObject *parent = 0);
    ~VideoThread();
    virtual void run();

signals:
    void foundCameras(QStringList);
    void gotFrame(QImage frame);

public slots:
    void openCamera(int index=0);
    void poll();

protected:
    videoInput * m_cams;
    int m_openCam;
    QTimer m_clock;
};

#endif // VIDEOTHREAD_H
