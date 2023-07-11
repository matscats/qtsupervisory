#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
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

public slots:
    void tcpConnect();
    void tcpDisconnect();
    void putData();
    void controlTimerStart();
    void controlTimerStop();
    void timerEvent(QTimerEvent *timer);

private:
    Ui::MainWindow *ui;
    QTcpSocket *socket;
    unsigned int temp;
};

#endif // MAINWINDOW_H
