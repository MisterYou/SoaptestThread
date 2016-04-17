#include "uploadthread.h"

UploadThread::UploadThread(QObject *parent) :
    QThread(parent),http(this)
{
    http.setHost("127.0.0.1",8080);
    http.setAction("http://127.0.0.1:8080/SmartHomeWebservice/services/SmartHomeService?wsdl");
}

void UploadThread::run()
{
    while(1)
    {
        QByteArray temp = myCom->readAll(); //读取串口缓冲区的所有数据给临时变量temp
        QString str,str1,str2,str3;
        QString judge;
        int wendu,shidu;

        if(!temp.isEmpty()){
          //   qDebug()<<temp;
             str =QString(temp);
          //   qDebug()<<str;
           //  if(!str.isEmpty()){
                   QStringList strlist = str.split(" ");
                   str1 = strlist[0];
                   str2 = strlist[1];

                    judge = str.mid(3,1);
                   // qDebug()<<judge;
                    //qDebug()<<str;
                    wendu=strlist[0].toInt();
                     shidu = strlist[1].toInt();
                   if(judge =="O"|| judge=="C"){
                       //myCom->write(str.toAscii());
                      // qDebug()<<" hello";
                    }else{
                           if(wendu>10 && wendu<35){
                           if(shidu>20&&shidu<50){
                              str3="normal";
                           }
                          else{
                              str3 = "warning";
                          }
                          }   else{
                            str3 = "warning";
                      }
                           QtSoapMessage request;
                           request.setMethod(QtSoapQName("shSer","http://webservice.sjtu.edu"));
                           request.addMethodArgument("str1","",str1);
                           request.addMethodArgument("str2","",str2);
                           request.addMethodArgument("str3","",str3);
                          http.submitRequest(request,"http://127.0.0.1:8080/SmartHomeWebservice/services/SmartHomeService?wsdl");
             }


         }
        this->msleep(100);
    }
}

void UploadThread::setMessage(Posix_QextSerialPort *openCom)
{
    myCom = openCom;
}
