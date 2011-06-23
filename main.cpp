#include <QtGui/QApplication>
#include "Display.h"
#include "VideoCapture.h"
#include "ThreadWaiter.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Display w;

    w.show();

    return a.exec();
}
