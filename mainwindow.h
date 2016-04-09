
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qtsoap.h>

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
    void submitRequest();
    void getResponse();
    
private:
    Ui::MainWindow *ui;
    QtSoapHttpTransport http;
};

#endif // MAINWINDOW_H
