#ifndef VIDEOCAPTURE_H
#define VIDEOCAPTURE_H

#include <QThread>
#include <QImage>
#include <QTimer>
#include <QStringList>

class videoInput;

class VideoCapture : public QObject
{
    Q_OBJECT
public:
    explicit VideoCapture(QThread * thread, QObject *parent = 0);
    ~VideoCapture();

signals:
    void foundCameras(QStringList);
    void gotFrame(QImage frame);
    void autoResolution(int w, int h);

public slots:
    void setupResolution(int w=0, int h=0);
    void scanCameras();
    void openCamera(int index=0);
    void closeCamera();

protected slots:
    void onThreadStarted();
    void onClockTick();

protected:
    videoInput * m_cams;
    int m_openCam, m_resW, m_resH;
    QTimer * m_clock;
};

#endif // VIDEOCAPTURE_H
