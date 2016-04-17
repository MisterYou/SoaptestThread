#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>
#include <QTimer>

class MyThread : public QThread
{
    Q_OBJECT
public:
    explicit MyThread(QObject *parent = 0);

    void run();
    void setTime(int times);

    
signals:
    void timeStart();

private:
    QTimer *readTime;
    int settimes;

private slots:
    void sendTime();


public slots:
    
};

#endif // MYTHREAD_H
