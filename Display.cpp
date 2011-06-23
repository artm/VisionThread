#include <QDebug>

#include "Display.h"
#include "ui_Display.h"

Display::Display(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Display)
{
    ui->setupUi(this);
    Q_ASSERT(connect(ui->videoResolution, SIGNAL(activated(QString)), SLOT(resolutionChosen(QString))));
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
        ui->videoControls->setEnabled(true);
        ui->videoDevices->addItems(devNames);
    } else {
        ui->videoControls->setEnabled(false);
        ui->videoDevices->addItem("No video input detected");
    }
}

void Display::resolutionChosen(QString resolution)
{
    QRegExp re("(\\d+)x(\\d+)");
    if (resolution.contains(re)) {
        emit resolutionChosen( re.cap(1).toInt(), re.cap(2).toInt() );
    } else {
        emit resolutionChosen(0,0);
    }
}

void Display::showResolution(int w, int h)
{
    QRegExp re(QString("^(.*\\D)?%1x%2(\\D.*)?$").arg(w).arg(h));
    for(int i=0; i<ui->videoResolution->count(); ++i) {
        QString resolution = ui->videoResolution->itemText(i);
        if (resolution.contains(re)) {
            ui->videoResolution->setCurrentIndex(i);
            return;
        }
    }
    ui->videoResolution->insertItem(1,QString("%1x%2 (chosen by OS)").arg(w).arg(h));
    ui->videoResolution->setCurrentIndex(1);
}
