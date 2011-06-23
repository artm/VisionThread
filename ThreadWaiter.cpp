#include "ThreadWaiter.h"
#include <QCoreApplication>
#include <QThread>


ThreadWaiter::ThreadWaiter(QObject *parent)
    : QObject(parent)
{
    Q_ASSERT( connect(qApp, SIGNAL(aboutToQuit()), SLOT(onAboutToQuit())) );
}

ThreadWaiter& ThreadWaiter::operator<<(QThread * thread)
{
    m_threads << thread;
    return *this;
}

ThreadWaiter& ThreadWaiter::operator<<(QThread & thread)
{
    return operator<<(&thread);
}

void ThreadWaiter::onAboutToQuit()
{
    foreach(QThread * thread, m_threads) {
        thread->quit();
        thread->wait(2000);
    }
}

QThread * ThreadWaiter::makeThread()
{
    QThread * thread = new QThread(this);
    operator <<(thread);
    return thread;
}
