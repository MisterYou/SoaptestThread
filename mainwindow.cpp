#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtGui/QMessageBox>
#include <QtGui/QCursor>
#include <QTextCodec>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),http(this),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowState(Qt::WindowMaximized);
   //  this->setWindowState(Qt::WindowFullScreen);

    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));//tr使用的编码
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf8"));//QString使用的编码，没有这一条
    //ui->pushButton->setText(QString::fromUtf8("离家"));
    /*
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("gb2312"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("gb2312"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("gb2312"));
    */
    this->setWindowTitle("智能家庭智能控制系统");

    QPalette bgpal = palette();
  //  bgpal.setColor (QPalette::Background, QColor (111,0 ,220, 255));
    bgpal.setColor (QPalette::Background,"#358A2B");
    this->setPalette(bgpal);
    //--------------设置位数-------//
    //ui->lcdNumber->setDigitCount(8);
   // ui->lcdNumber_2->setDigitCount(8);

 // setStyleSheet("QLCDNumber{color: red}");
/*
    ui->pushButton->setFont(QFont("",36,QFont::Black));
    ui->pushButton_2->setFont(QFont("",36,QFont::Black));
    ui->pushButton_4->setFont(QFont("",36,QFont::Black));
    ui->pushButton_3->setFont(QFont("",36,QFont::Black));
    ui->pushButton_9->setFont(QFont("",36,QFont::Black));
    ui->label->setFont(QFont("",36,QFont::Black));
    ui->label_2->setFont(QFont("",36,QFont::Black));
    */
  //  ui->groupBox_;

    //-------------------------------打开串口----------------------------//
    myCom = new Posix_QextSerialPort("/dev/ttyUSB0",QextSerialBase::Polling);//定义串口对象，指名串口名和查询模式，使用polling
    myCom ->open(QIODevice::ReadWrite);     //以读写方式打开串口
    myCom->setBaudRate(BAUD115200);     //波特率设置，我们设置为115200

    myCom->setDataBits(DATA_8);     //数据位设置，我们设置为8位数据位

    myCom->setParity(PAR_NONE);     //奇偶校验设置，我们设置为无校验

    myCom->setStopBits(STOP_1);     //停止位设置，我们设置为1位停止位

    myCom->setFlowControl(FLOW_OFF);     //数据流控制设置，我们设置为无数据流控制

    myCom->setTimeout(200);   //延时设置，我们设置为延时200ms,如果设置为500ms的话，会造成程序无响应，原因未知

    dialog.setCom(myCom);



    gprsthread = new GPRSThread(this);
//    connect(&http,SIGNAL(responseReady()),this,SLOT(getResponse()));
/*
    uploadThread = new UploadThread(this);
    uploadThread->setMessage(myCom);
    uploadThread->start();
*/

    inputThread = new InputThread(this);
    inputThread->setMessage(myCom);
    inputThread->start();

     http.setHost("192.168.0.100",8080);
    http.setAction("http://192.168.0.100:8080/SmartHomeWebservice/services/SmartHomeService?wsdl");

    //---------------------------上传数据----------------------------//

    readTimer = new QTimer(this);//设置读取计时器
    readTimer->start(100);//设置延时为100ms
    connect(readTimer,SIGNAL(timeout()),this,SLOT(readMyCom()));//信号和槽函数关联，当达到定时时间时，进行读串口操作

/*******************************************  采用两个定时器形式
//--------------------------------读取指令-----------------------//
    inputTimer = new QTimer(this);
    inputTimer->start(1000);
    connect(inputTimer,SIGNAL(timeout()),this,SLOT(inputMyCom()));

 *********************************************************/

//-------------------------------访问webservice--------------------------//

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
/*
void MainWindow::getResponse()
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
             ui->ouputEdit->append("invalid return value");
             return;
         }
        //ui->ouputEdit->append(res.toString());
        qDebug() << res.toString();

        str = res.toString();
        QStringList strlist = str.split(" ");
        if(strlist[0] == "0"){
            qDebug()<<"No order";
            ui->textEdit->append("No order");
        }else if(strlist[0] == "1"){
            if(strlist[1] == "air-conditioning"){
                strorder = "KT "+strlist[2];
              //  qDebug()<<strlist[2].length();
                ui->textEdit->append("insert success");
                myCom->write(strorder.toAscii());
            } else if(strlist[1] == "light"){
                strorder = "LT "+strlist[2];
                myCom->write(strorder.toAscii());
                ui->textEdit->append("insert success");
            } else if(strlist[1] == "television"){
                strorder = "TV "+strlist[2];
                myCom->write(strorder.toAscii());
                ui->textEdit->append("insert success");
            }

        }else if(strlist[0] == "2"){
             qDebug()<<"Have some error";
        }
     }

}

*/
void MainWindow::readMyCom()
{
    QByteArray temp = myCom->readAll(); //读取串口缓冲区的所有数据给临时变量temp
    QString str,str1,str2,str3;
    QString judge;
    int wendu,shidu;


    if(!temp.isEmpty()){
         str =QString(temp);

         QStringList strlist = str.split(" ");
         str1 = strlist[0];
         str2 = strlist[1];
       // ui->textEdit->append(str);
          judge = str.mid(3,1);
        //  qDebug()<<str;
          wendu=strlist[0].toInt();
           shidu = strlist[1].toInt();
               if(judge !="O"&& judge !="C"){
          //         ui->lcdNumber->display(strlist[0]+"°C");

          //         ui->lcdNumber_2->display(strlist[1]+"RH%");
/*
                   QPalette pe;
                   pe.setColor(QPalette::WindowText,Qt::white);
                   ui->label_3->setPalette(pe);
                   QPalette pe1;
                   pe1.setColor(QPalette::WindowText,Qt::white);
                   ui->label_4->setPalette(pe1);
*/
                   ui->label_3->setText(strlist[0]+"°C");
                   ui->label_4->setText(strlist[1]+"RH%");

                   gprsthread->setMessage(str);
                   gprsthread->start();
                       if(wendu>10 && wendu<35){
                       if(shidu>20&&shidu<50){
                          str3="normal";
               //           ui->lcdNumber->setPalette(Qt::white);
               //           ui->lcdNumber_2->setPalette(Qt::white);
                          QPalette pe2;
                          pe2.setColor(QPalette::WindowText,Qt::white);
                          ui->label_3->setPalette(pe2);
                          QPalette pe3;
                          pe3.setColor(QPalette::WindowText,Qt::white);
                          ui->label_4->setPalette(pe3);

                       }
                      else{
                          str3 = "warning";
                       //   ui->lcdNumber->setPalette(Qt::red);
                       //    ui->lcdNumber_2->setPalette(Qt::red);
                          QPalette pe4;
                          pe4.setColor(QPalette::WindowText,Qt::red);
                          ui->label_4->setPalette(pe4);
                      }
                      }   else{
                           str3 = "warning";

                                 if(shidu>20&&shidu<50){
                                     QPalette pe5;
                                     pe5.setColor(QPalette::WindowText,Qt::red);
                                     ui->label_3->setPalette(pe5);
                                     QPalette pe6;
                                     pe6.setColor(QPalette::WindowText,Qt::white);
                                     ui->label_4->setPalette(pe6);
                                 }
                                 else{
                                     QPalette pe7;
                                     pe7.setColor(QPalette::WindowText,Qt::red);
                                     ui->label_3->setPalette(pe7);
                                     QPalette pe8;
                                     pe8.setColor(QPalette::WindowText,Qt::red);
                                     ui->label_4->setPalette(pe8);
                                 }


                //        ui->lcdNumber->setPalette(Qt::red);
                  //       ui->lcdNumber_2->setPalette(Qt::red);

                  }
                       QtSoapMessage request;
                       request.setMethod(QtSoapQName("shSer","http://webservice.sjtu.edu"));
                       request.addMethodArgument("str1","",str1);
                       request.addMethodArgument("str2","",str2);
                       request.addMethodArgument("str3","",str3);
                      http.submitRequest(request,"http://192.168.0.100:8080/SmartHomeWebservice/services/SmartHomeService?wsdl");



     }
     //   http.submitRequest(request,"http://192.168.0.100:8080/SmartHomeWebservice/services/SmartHomeService?wsdl");

  }
}
/*
void MainWindow::inputMyCom()
{
    QtSoapMessage request;
    request.setMethod(QtSoapQName("getOrder","http://webservice.sjtu.edu"));
    http.submitRequest(request,"http://192.168.0.100:8080/SmartHomeWebservice/services/SmartHomeService?wsdl");
}
*/


void MainWindow::on_pushButton_4_clicked()
{
    this->close();
}

void MainWindow::on_pushButton_3_clicked()
{
    this->hide();
    dialog.show();
    dialog.exec();
    this->show();
}
//----------------------回家模式----------------------//
void MainWindow::on_pushButton_2_clicked()
{
    QString str ;
      str= "KT O";
    myCom->write(str.toAscii());
}
//---------------------离家模式----------------------//
void MainWindow::on_pushButton_clicked()
{
    QString str;
    str= "KT C";
    myCom->write(str.toAscii());
}
//-------------------------报警----------------------//
void MainWindow::on_pushButton_9_clicked()
{

}
