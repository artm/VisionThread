#ifndef VIDEOCAPTURECONTROLS_H
#define VIDEOCAPTURECONTROLS_H

#include <QWidget>

class QComboBox;
class VideoCapture;
class ThreadWaiter;

class VideoCaptureControls : public QWidget
{
    Q_OBJECT
public:
    explicit VideoCaptureControls(QWidget *parent = 0);
    ~VideoCaptureControls();

    VideoCapture * capture();

signals:
    void resolutionChosen(int w, int h);

public slots:
    void populateDeviceList(const QStringList& devNames);
    // user has chosen via combo
    void resolutionChosen(QString resolution);
    // OS has chosen - should reflect in the combo
    void showResolution(int w, int h);

protected:
    QComboBox * m_deviceSelector, * m_resolutionSelector;
    VideoCapture * m_capture;
    ThreadWaiter * m_waiter;
};

#endif // VIDEOCAPTURECONTROLS_H
