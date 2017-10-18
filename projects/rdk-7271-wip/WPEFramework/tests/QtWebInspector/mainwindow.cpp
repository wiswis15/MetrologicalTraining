#include "mainwindow.h"
//#include "ui_mainwindow.h"
#include "WebInspectorConnection.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
//    , ui(new Ui::MainWindow)
{
//    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
//    delete ui;
}

void MainWindow::on__connect_clicked()
{
    if (_connection == NULL) {
        // This is a request for a connect
        _connection = new WPEFramework::WebInspectorConnection("echo.testserver.org", 80, "", false);

        ASSERT(_connection != NULL);

        _connection->Connect();
    }
    else {
        // This is a request for a disconnect
        _connection->Disconnect();
    }
}

void MainWindow::on__send_clicked()
{
}
