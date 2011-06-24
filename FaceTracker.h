#ifndef FACETRACKER_H
#define FACETRACKER_H

#include <QObject>
#include <QImage>

class FaceTracker : public QObject {
    Q_OBJECT
public:
    explicit FaceTracker(QObject * parent = 0);
    virtual ~FaceTracker();

signals:
    void facesDetected(const QImage& frame, QList<QRect> faces);

public slots:
    void detect(const QImage& frame);

private:
    struct Private;
    Private * m_private;
};


#endif // FACETRACKER_H
