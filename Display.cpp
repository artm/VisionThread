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
