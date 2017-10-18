#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <generics/generics.h>
#include <websocket/websocket.h>

//namespace Ui {
//class MainWindow;
//}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();

private slots:
    void on__connect_clicked();

    void on__send_clicked();

private:
//    Ui::MainWindow* ui;
    WPEFramework::Web::TextWebSocket* _connection;
};

#endif // MAINWINDOW_H
