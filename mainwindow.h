
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qtsoap.h>
#include <QTimer>
#include <posix_qextserialport.h>
#include <qextserialbase.h>
#include <QDebug>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private slots:
    //void submitRequest();//调用接口
    void getResponse();//接收webservice返回的数据

    void readMyCom();//读取串口

    
private:
    Ui::MainWindow *ui;
    QtSoapHttpTransport http;
    Posix_QextSerialPort *myCom;//串口
    QTimer *readTimer;//定时器


};

#endif // MAINWINDOW_H
