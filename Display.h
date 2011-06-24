#ifndef DISPLAY_H
#define DISPLAY_H

#include <QMainWindow>

class FaceTracker;

namespace Ui {
    class Display;
}

class Display : public QMainWindow
{
    Q_OBJECT

public:
    explicit Display(QWidget *parent = 0);
    ~Display();

signals:

public slots:
    void showFrame(const QImage& frame);
    void showFaces(const QImage& frame, QList<QRect> faces);

private:
    Ui::Display *ui;
    FaceTracker * m_tracker;
};

#endif // DISPLAY_H
