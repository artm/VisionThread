#ifndef VIDEOCAPTURE_H
#define VIDEOCAPTURE_H

#include <QThread>
#include <QImage>
#include <QTimer>
#include <QStringList>

class VideoCapture : public QObject
{
    Q_OBJECT
public:
    explicit VideoCapture(QThread * thread = 0, QObject *parent = 0);
    ~VideoCapture();

signals:
    void foundDevices(QStringList);
    void gotFrame(QImage frame);
    void autoResolution(int w, int h);

public slots:
    void setupResolution(int w=0, int h=0);
    void scanForDevices();
    void openDevice(int index=0);
    void closeDevice();

protected slots:
    virtual void onThreadStarted();
    virtual void onClockTick();

protected:
    struct Private;
    Private * m_private;
    int m_deviceCount;
    int m_openDevice, m_resW, m_resH;
    QTimer * m_clock;
};

#endif // VIDEOCAPTURE_H
