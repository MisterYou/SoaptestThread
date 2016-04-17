#include "mythread.h"
#include  <QtCore>

MyThread::MyThread(QObject *parent) :
    QThread(parent)
{
}

void MyThread::run()
{

}

void MyThread::setTime(int times)
{
    settimes = times;
}

void MyThread::sendTime()
{
    emit timeStart();
}
