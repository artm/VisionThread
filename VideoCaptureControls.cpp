#include "VideoCaptureControls.h"
#include "VideoCapture.h"
#include "ThreadWaiter.h"

#include <QGridLayout>
#include <QComboBox>

VideoCaptureControls::VideoCaptureControls(QWidget *parent) :
    QWidget(parent)
{
    QGridLayout * layout = new QGridLayout(this);

    m_deviceSelector = new QComboBox();
    layout->addWidget(m_deviceSelector, 0, 0);

    m_resolutionSelector = new QComboBox();
    layout->addWidget(m_resolutionSelector, 0, 1);
    m_resolutionSelector->addItem("Let OS decide");
    m_resolutionSelector->addItem("320x240");
    m_resolutionSelector->addItem("640x480");

    m_waiter = new ThreadWaiter(this);
    m_capture = new VideoCapture(m_waiter->makeThread());

    Q_ASSERT(connect(m_capture, SIGNAL(foundCameras(QStringList)), SLOT(populateDeviceList(QStringList))));
    Q_ASSERT(connect(m_capture, SIGNAL(autoResolution(int,int)), SLOT(showResolution(int, int))));
    Q_ASSERT(connect(this, SIGNAL(resolutionChosen(int,int)), m_capture, SLOT(setupResolution(int,int))));
    Q_ASSERT(connect(m_resolutionSelector, SIGNAL(activated(QString)), SLOT(resolutionChosen(QString))));
    Q_ASSERT(connect(m_deviceSelector, SIGNAL(activated(int)), m_capture, SLOT(openCamera(int))));

    m_capture->thread()->start();
}

VideoCaptureControls::~VideoCaptureControls()
{
    delete m_capture;
}

void VideoCaptureControls::populateDeviceList(const QStringList &devNames)
{
    m_deviceSelector->clear();
    if (devNames.size()>0) {
        setEnabled(true);
        m_deviceSelector->addItems(devNames);
    } else {
        setEnabled(false);
        m_deviceSelector->addItem("No video input detected");
    }
}

void VideoCaptureControls::resolutionChosen(QString resolution)
{
    QRegExp re("(\\d+)x(\\d+)");
    if (resolution.contains(re)) {
        emit resolutionChosen( re.cap(1).toInt(), re.cap(2).toInt() );
    } else {
        emit resolutionChosen(0,0);
    }
}

void VideoCaptureControls::showResolution(int w, int h)
{
    QRegExp re(QString("^(.*\\D)?%1x%2(\\D.*)?$").arg(w).arg(h));
    for(int i=0; i<m_resolutionSelector->count(); ++i) {
        QString resolution = m_resolutionSelector->itemText(i);
        if (resolution.contains(re)) {
            m_resolutionSelector->setCurrentIndex(i);
            return;
        }
    }
    m_resolutionSelector->insertItem(1,QString("%1x%2 (chosen by OS)").arg(w).arg(h));
    m_resolutionSelector->setCurrentIndex(1);
}

VideoCapture * VideoCaptureControls::capture()
{
    return m_capture;
}


