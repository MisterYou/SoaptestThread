#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtGui/QMessageBox>
#include <QtGui/QCursor>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),http(this),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //http = new QtSoapHttpTransport;
    connect(&http,SIGNAL(responseReady()),this,SLOT(getResponse()));
    connect(ui->startButton,SIGNAL(clicked()),this,SLOT(submitRequest()));
/*
    QtSoapMessage request;
    request.setMethod(QtSoapQName("plus","http://edu.sjtu.webservice"));
    // request.addMethodArgument("x","",ui->inputEdit->text().toInt());
    request.addMethodArgument("x","",3);
*/
    //  http.setHost("192.168.0.101",8080);
    http.setHost("127.0.0.1",8080);
    //http.setAction("http://10.24.20.101:8080/Test/services/CalculateService?wsdl");
    http.setAction("http://127.0.0.1:8080/SmartHomeWebservice/services/SmartHomeService?wsdl");
  //  http.submitRequest(request,"http://192.168.0.101:8080/Test/services/CalculateService?wsdl");


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
   // request.setMethod(QtSoapQName("shSer","http://edu.sjtu.webservice"));
    request.setMethod(QtSoapQName("shSer","http://webservice.sjtu.edu"));
    // request.addMethodArgument("x","",ui->inputEdit->text().toInt());
    request.addMethodArgument("str1","",ui->inputEdit->text());
     request.addMethodArgument("str2","",ui->inputEdit_2->text());
    //request.addMethodArgument("num2","",ui->inputEdit_2->text().toFloat());
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















