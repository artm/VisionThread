#ifndef DISPLAY_H
#define DISPLAY_H

#include <QMainWindow>

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
    void resolutionChosen(int w, int h);

public slots:
    void showFrame(const QImage& frame);
    void populateDeviceList(const QStringList& devNames);
    // user has chosen via combo
    void resolutionChosen(QString resolution);
    // OS has chosen - should reflect in the combo
    void showResolution(int w, int h);

private:
    Ui::Display *ui;
};

#endif // DISPLAY_H
