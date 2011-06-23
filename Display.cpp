#include <QDebug>
#include <QSettings>

#include "Display.h"
#include "ui_Display.h"
#include "VideoCapture.h"

Display::Display(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Display)
{
    ui->setupUi(this);
    Q_ASSERT(connect( ui->captureControls->capture(), SIGNAL(gotFrame(QImage)), SLOT(showFrame(QImage))));

    QSettings s("V2_Lab", "VisionThreadDemo");
    ui->captureControls->load(s);
}

Display::~Display()
{
    QSettings s("V2_Lab", "VisionThreadDemo");
    ui->captureControls->save(s);
    delete ui;
}

void Display::showFrame(const QImage &frame)
{
    ui->videoLabel->setPixmap( QPixmap::fromImage(frame) );
}

