#ifndef VIDEOCAPTURECONTROLS_H
#define VIDEOCAPTURECONTROLS_H

#include <QWidget>

class QComboBox;
class VideoCapture;
class ThreadWaiter;
class QSettings;

class VideoCaptureControls : public QWidget
{
    Q_OBJECT
public:
    explicit VideoCaptureControls(QWidget *parent = 0);
    ~VideoCaptureControls();

    VideoCapture * capture();
    void save(QSettings& s);
    void load(QSettings& s);

signals:
    void deviceChosen(int index);
    void resolutionChosen(int w, int h);

public slots:
    void populateDeviceList(const QStringList& devNames);
    void onDeviceChosen(int id);
    // user has chosen via combo
    void onResolutionChosen(QString resolution);
    // OS has chosen - should reflect in the combo
    void showResolution(int w, int h);

protected:
    QComboBox * m_deviceSelector, * m_resolutionSelector;
    VideoCapture * m_capture;
    ThreadWaiter * m_waiter;
};

#endif // VIDEOCAPTURECONTROLS_H
