#ifndef INPUTTHREAD_H
#define INPUTTHREAD_H

#include <QThread>
#include "posix_qextserialport.h"
#include "qextserialbase.h"
#include <qtsoap.h>

class InputThread : public QThread
{
    Q_OBJECT
public:
    explicit InputThread(QObject *parent = 0);

    void run();
    void setMessage(Posix_QextSerialPort *openCom);
    
signals:

private:
    Posix_QextSerialPort *myCom;
    QtSoapHttpTransport http;

public slots:
    void getResponse();//接收webservice返回的数据

    
};

#endif // INPUTTHREAD_H
