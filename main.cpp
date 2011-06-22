#include <QtGui/QApplication>
#include "Display.h"
#include "VideoThread.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    VideoThread v;
    Display w;

    Q_ASSERT(w.connect(&v, SIGNAL(foundCameras(QStringList)), &v, SLOT(openCamera())));
    Q_ASSERT(w.connect(&v, SIGNAL(gotFrame(QImage)), &w, SLOT(showFrame(QImage)), Qt::QueuedConnection));
    v.start();
    w.show();

    return a.exec();
}
