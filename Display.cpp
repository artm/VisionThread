#include <QDebug>

#include "Display.h"
#include "ui_Display.h"

Display::Display(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Display)
{
    ui->setupUi(this);
}

Display::~Display()
{
    delete ui;
}

void Display::showFrame(const QImage &frame)
{
    ui->videoLabel->setPixmap( QPixmap::fromImage(frame) );
}

void Display::populateDeviceList(const QStringList &devNames)
{
    ui->videoDevices->clear();
    if (devNames.size()>0) {
        ui->videoDevices->setEnabled(true);
        ui->videoDevices->addItems(devNames);
    } else {
        ui->videoDevices->setEnabled(false);
        ui->videoDevices->addItem("No video input detected");
    }
}
