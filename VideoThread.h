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


signals:
    void foundCameras(QStringList);
    void gotFrame(QImage frame);
    void autoResolution(int w, int h);

public slots:
    void searchCameras();
    void openCamera(int index=0);
    void closeCamera();
    void poll();
    void shutdown();

    void setupResolution(int w=0, int h=0);

protected:
    virtual void run();

    videoInput * m_cams;
    int m_openCam, m_resW, m_resH;
    QTimer m_clock;
};

#endif // VIDEOTHREAD_H
