#ifndef THREADWAITER_H
#define THREADWAITER_H

#include <QObject>
#include <QList>

/* A helper object that shuts down threads when the application is about to quit */

class QThread;

class ThreadWaiter : public QObject
{
    Q_OBJECT
public:
    explicit ThreadWaiter(QObject *parent = 0);

    ThreadWaiter& operator<<(QThread *);
    ThreadWaiter& operator<<(QThread &);

signals:

public slots:
    void onAboutToQuit();

protected:

    QList<QThread*> m_threads;
};

#endif // THREADWAITER_H
