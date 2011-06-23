#include <QDebug>

#include "Display.h"
#include "ui_Display.h"
#include "VideoCapture.h"

Display::Display(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Display)
{
    ui->setupUi(this);

    Q_ASSERT(connect( ui->captureControls->capture(), SIGNAL(gotFrame(QImage)), SLOT(showFrame(QImage))));
}

Display::~Display()
{
    delete ui;
}

void Display::showFrame(const QImage &frame)
{
    ui->videoLabel->setPixmap( QPixmap::fromImage(frame) );
}

