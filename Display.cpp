#include <QDebug>
#include <QSettings>
#include <QPainter>

#include "Display.h"
#include "ui_Display.h"
#include "VideoCapture.h"
#include "FaceTracker.h"


Display::Display(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Display)
{
    ui->setupUi(this);
    Q_ASSERT(connect( ui->captureControls->capture(), SIGNAL(gotFrame(QImage)), SLOT(showFrame(QImage))));

    QSettings s("V2_Lab", "VisionThreadDemo");
    ui->captureControls->load(s);

    m_tracker = new FaceTracker;
    // this also moves it to video thread
    m_tracker->moveToThread(ui->captureControls->capture()->thread());
    Q_ASSERT(connect(ui->captureControls->capture(), SIGNAL(gotFrame(QImage)), m_tracker, SLOT(detect(QImage))));
    Q_ASSERT(connect(m_tracker, SIGNAL(facesDetected(QImage,QList<QRect>)), SLOT(showFaces(QImage,QList<QRect>))));
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

void Display::showFaces(const QImage &frame, QList<QRect> faces)
{
    QPixmap pix = QPixmap::fromImage(frame);
    QPainter p(&pix);

    p.setPen(Qt::white);
    foreach(QRect rect, faces)
        p.drawRect(rect);
    ui->facesLabel->setPixmap( pix );
}

