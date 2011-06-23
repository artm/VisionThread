#include <QtGui/QApplication>
#include "Display.h"
#include "VideoCapture.h"
#include "ThreadWaiter.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ThreadWaiter waiter;
    VideoCapture v(waiter.makeThread());

    Display w;

    Q_ASSERT(QObject::connect(&v, SIGNAL(gotFrame(QImage)), &w, SLOT(showFrame(QImage))));
    Q_ASSERT(QObject::connect(&v, SIGNAL(foundCameras(QStringList)), &w, SLOT(populateDeviceList(QStringList))));
    Q_ASSERT(QObject::connect(&v, SIGNAL(autoResolution(int,int)), &w, SLOT(showResolution(int, int))));
    Q_ASSERT(QObject::connect(&w, SIGNAL(resolutionChosen(int,int)), &v, SLOT(setupResolution(int,int))));
    Q_ASSERT(QObject::connect(&v, SIGNAL(foundCameras(QStringList)), &v, SLOT(openCamera())));

    v.thread()->start();
    w.show();

    return a.exec();
}
