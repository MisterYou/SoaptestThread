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

//-------------------------------访问webservice--------------------------//
/*
  //  connect(&http,SIGNAL(responseReady()),this,SLOT(getResponse()));
    //connect(ui->startButton,SIGNAL(clicked()),this,SLOT(submitRequest()));

    QtSoapMessage request;
    request.setMethod(QtSoapQName("plus","http://edu.sjtu.webservice"));
    // request.addMethodArgument("x","",ui->inputEdit->text().toInt());
    request.addMethodArgument("x","",3);

    //  http.setHost("192.168.0.101",8080);
    http.setHost("127.0.0.1",8080);
    http.setAction("http://127.0.0.1:8080/SmartHomeWebservice/services/SmartHomeService?wsdl");
  //  http.submitRequest(request,"http://192.168.0.101:8080/Test/services/CalculateService?wsdl");

*/
}

MainWindow::~MainWindow()
{
    delete ui;
}



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
    request.setMethod(QtSoapQName("shSer","http://webservice.sjtu.edu"));
    request.addMethodArgument("str1","",ui->inputEdit->text());
     request.addMethodArgument("str2","",ui->inputEdit_2->text());
    http.submitRequest(request,"http://127.0.0.1:8080/SmartHomeWebservice/services/SmartHomeService?wsdl");
   // QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

}

void MainWindow::getResponse()
{
     const QtSoapMessage &message = http.getResponse();
     if (message.isFault()) {
       //qDebug("Error: %s", message.faultString().value().toString().toLatin1().constData());
         qDebug("Error: %s", qPrintable(message.faultString().toString()));
    }
     else{
         const QtSoapType &res = message.returnValue();
         if(!res.isValid()){
             ui->ouputEdit->text().append("invalid return value");
             return;
         }
         ui->ouputEdit->setText(res.toString());
         qDebug() << res.toString();
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
/*
         connect(&http,SIGNAL(responseReady()),this,SLOT(getResponse()));

        QtSoapMessage request;
        request.setMethod(QtSoapQName("shSer","http://webservice.sjtu.edu"));
        request.addMethodArgument("str1","",str1);
         request.addMethodArgument("str2","",str2);
         http.setHost("127.0.0.1",8080);
         http.setAction("http://127.0.0.1:8080/SmartHomeWebservice/services/SmartHomeService?wsdl");
        http.submitRequest(request,"http://127.0.0.1:8080/SmartHomeWebservice/services/SmartHomeService?wsdl");
*/
    }
}














