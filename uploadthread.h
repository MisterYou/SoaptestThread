#ifndef UPLOADTHREAD_H
#define UPLOADTHREAD_H

#include <QThread>
#include "posix_qextserialport.h"
#include "qextserialbase.h"
#include <qtsoap.h>


class UploadThread : public QThread
{
    Q_OBJECT
public:
    explicit UploadThread(QObject *parent = 0);
    void run();
    void setMessage(Posix_QextSerialPort *openCom);
    
signals:


private:
    Posix_QextSerialPort *myCom;
    QtSoapHttpTransport http;

public slots:
    
};

#endif // UPLOADTHREAD_H
