#include <QtGui/QApplication>
#include "Display.h"
#include "VideoThread.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Display w;
    VideoThread v;

    Q_ASSERT(v.connect(&v, SIGNAL(foundCameras(QStringList)), SLOT(openCamera())));

    Q_ASSERT(w.connect(&v, SIGNAL(gotFrame(QImage)), SLOT(showFrame(QImage)), Qt::QueuedConnection));
    Q_ASSERT(w.connect(&v, SIGNAL(foundCameras(QStringList)), SLOT(populateDeviceList(QStringList)),
             Qt::QueuedConnection));

    v.start();
    w.show();

    return a.exec();
}
