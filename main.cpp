#include <QtGui/QApplication>
#include "Display.h"
#include "VideoThread.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Display w;
    VideoThread v;

    Q_ASSERT(w.connect(&v, SIGNAL(gotFrame(QImage)), SLOT(showFrame(QImage)), Qt::QueuedConnection));
    Q_ASSERT(w.connect(&v, SIGNAL(foundCameras(QStringList)), SLOT(populateDeviceList(QStringList)),
             Qt::QueuedConnection));
    Q_ASSERT(w.connect(&v, SIGNAL(autoResolution(int,int)), SLOT(showResolution(int, int))));

    Q_ASSERT(v.connect(&w, SIGNAL(resolutionChosen(int,int)), SLOT(setupResolution(int,int))));

    // autostart
    Q_ASSERT(v.connect(&v, SIGNAL(foundCameras(QStringList)), SLOT(openCamera())));

    v.start();
    w.show();

    return a.exec();
}
