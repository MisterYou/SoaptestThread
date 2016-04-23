#include "inputthread.h"

InputThread::InputThread(QObject *parent) :
    QThread(parent),http(this)
{
    http.setHost("192.168.0.100",8080);
    http.setAction("http://192.168.0.100:8080/SmartHomeWebservice/services/SmartHomeService?wsdl");
    connect(&http,SIGNAL(responseReady()),this,SLOT(getResponse()));
}

void InputThread::run()
{
    while(1){
        QtSoapMessage request;
        request.setMethod(QtSoapQName("getOrder","http://webservice.sjtu.edu"));
        http.submitRequest(request,"http://192.168.0.100:8080/SmartHomeWebservice/services/SmartHomeService?wsdl");
        this->msleep(2000);
    }
}

void InputThread::setMessage(Posix_QextSerialPort *openCom)
{
    myCom = openCom;
}

void InputThread::getResponse()
{
    QString str,strorder;
      const QtSoapMessage &message = http.getResponse();
     if (message.isFault()) {
         qDebug("Error: %s", message.faultString().value().toString().toLatin1().constData());
         qDebug("Error: %s", qPrintable(message.faultString().toString()));
    }
     else{
      //   const QtSoapType &res = message.returnValue();
        const QtSoapType &res = message.returnValue();
         if(!res.isValid()){
            qDebug()<<"invalid return value";
             return;
         }
        //ui->ouputEdit->append(res.toString());
      //  qDebug() << res.toString();

        str = res.toString();
        QStringList strlist = str.split(" ");
        if(strlist[0] == "0"){
            qDebug()<<"No order";
        }else if(strlist[0] == "1"){
            if(strlist[1] == "air-conditioning"){
                strorder = "KT "+strlist[2];
              //  qDebug()<<strlist[2].length();
                qDebug()<<"insert success";
                qDebug()<<str;
                myCom->write(strorder.toAscii());
            } else if(strlist[1] == "light"){
                strorder = "LT "+strlist[2];
                myCom->write(strorder.toAscii());
               qDebug()<<"insert success";
            } else if(strlist[1] == "television"){
                strorder = "TV "+strlist[2];
                myCom->write(strorder.toAscii());
                 qDebug()<<"insert success";
            }

        }else if(strlist[0] == "2"){
             qDebug()<<"Have some error";
        }
     }
}
