#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtGui/QMessageBox>
#include <QtGui/QCursor>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),http(this),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //-------------------------------打开串口----------------------------//
    myCom = new Posix_QextSerialPort("/dev/ttyUSB0",QextSerialBase::Polling);//定义串口对象，指名串口名和查询模式，使用polling
    myCom ->open(QIODevice::ReadWrite);     //以读写方式打开串口
    myCom->setBaudRate(BAUD115200);     //波特率设置，我们设置为115200

    myCom->setDataBits(DATA_8);     //数据位设置，我们设置为8位数据位

    myCom->setParity(PAR_NONE);     //奇偶校验设置，我们设置为无校验

    myCom->setStopBits(STOP_1);     //停止位设置，我们设置为1位停止位

    myCom->setFlowControl(FLOW_OFF);     //数据流控制设置，我们设置为无数据流控制

    myCom->setTimeout(200);   //延时设置，我们设置为延时200ms,如果设置为500ms的话，会造成程序无响应，原因未知
    readTimer = new QTimer(this);//设置读取计时器
    readTimer->start(100);//设置延时为100ms
    connect(readTimer,SIGNAL(timeout()),this,SLOT(readMyCom()));//信号和槽函数关联，当达到定时时间时，进行读串口操作

    inputTimer = new QTimer(this);
    inputTimer->start(1000);
    connect(inputTimer,SIGNAL(timeout()),this,SLOT(inputMyCom()));
//-------------------------------访问webservice--------------------------//

   connect(&http,SIGNAL(responseReady()),this,SLOT(getResponse()));
   //connect(ui->startButton,SIGNAL(clicked()),this,SLOT(submitRequest()));
/*---------------------测试------------------------------
    QtSoapMessage request;
    request.setMethod(QtSoapQName("plus","http://edu.sjtu.webservice"));
    // request.addMethodArgument("x","",ui->inputEdit->text().toInt());
    request.addMethodArgument("x","",3);
-----------------------------------------------------------*/
    //  http.setHost("192.168.0.101",8080);

    http.setHost("127.0.0.1",8080);
    http.setAction("http://127.0.0.1:8080/SmartHomeWebservice/services/SmartHomeService?wsdl");
    // http.setAction("http://127.0.0.1:8080/Test/services/CalculateService?wsdl");



   // http.setHost("192.168.0.100",8080);
   // http.setAction("http://192.168.0.100:8080/SmartHomeWebservice/services/SmartHomeService?wsdl");

}

MainWindow::~MainWindow()
{
    delete ui;
}


/*--------------------------按钮测试----------------------------
void MainWindow::submitRequest()
{
    if(ui->inputEdit->text() == ""||ui->inputEdit_2->text() == "")
    {
        QMessageBox::warning(this, tr("Missing license key"),
                     tr("Please enter your Google Web APIs license key."
                    " If you do not have one, you can visit<br>"
                    " http://api.google.com/ and create a Google"
                    " account to obtain a license key."));
        return;
    }
    QtSoapMessage request;
    request.setMethod(QtSoapQName("plus","http://webservice.sjtu.edu"));
    request.addMethodArgument("x","",ui->inputEdit->text().toFloat());
     request.addMethodArgument("y","",ui->inputEdit_2->text().toFloat());
    http.submitRequest(request,"http://127.0.0.1:8080/Test/services/CalculateService?wsdl");
   // QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

}
---------------------------------------------------------------------------------*/

void MainWindow::getResponse()
{
    QString str;
      const QtSoapMessage &message = http.getResponse();
     if (message.isFault()) {
         qDebug("Error: %s", message.faultString().value().toString().toLatin1().constData());
         qDebug("Error: %s", qPrintable(message.faultString().toString()));
    }
     else{
      //   const QtSoapType &res = message.returnValue();
        const QtSoapType &res = message.returnValue();
         if(!res.isValid()){
             ui->ouputEdit->append("invalid return value");
             return;
         }
        ui->ouputEdit->append(res.toString());
        //qDebug() << res.toString();

        str = res.toString();
        QStringList strlist = str.split(" ");
        if(strlist[0] == "0"){
            qDebug()<<"No order";
        }else if(strlist[0] == "1"){
            myCom->write(strlist[1].toAscii());
        }else if(strlist[0] == "2"){
             qDebug()<<"Have some error";
        }else if(strlist[0] == "3"){
            qDebug()<<"upload success";
       }else if(strlist[0] == "4"){
            qDebug()<<"upload error";
       }



     }

}

void MainWindow::readMyCom()
{
    QByteArray temp = myCom->readAll(); //读取串口缓冲区的所有数据给临时变量temp
    QString str,str1,str2;
    if(!temp.isEmpty()){
         str =QString(temp);
         QStringList strlist = str.split(" ");
         str1 = strlist[0];
         str2 = strlist[1];
         qDebug()<<str1;
         qDebug()<<str2;

     //    connect(&http,SIGNAL(responseReady()),this,SLOT(getResponse()));

         QtSoapMessage request;
         request.setMethod(QtSoapQName("shSer","http://webservice.sjtu.edu"));
        request.addMethodArgument("str1","",str1);
         request.addMethodArgument("str2","",str2);
        http.submitRequest(request,"http://127.0.0.1:8080/SmartHomeWebservice/services/SmartHomeService?wsdl");

     //   http.submitRequest(request,"http://192.168.0.100:8080/SmartHomeWebservice/services/SmartHomeService?wsdl");

    }
}

void MainWindow::inputMyCom()
{
    QtSoapMessage request;
    request.setMethod(QtSoapQName("getOrder","http://webservice.sjtu.edu"));
    http.submitRequest(request,"http://192.168.0.100:8080/SmartHomeWebservice/services/SmartHomeService?wsdl");
}














