#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QDebug>
#include <QListWidget>
#include "plotter.h"

namespace Ui
{
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
    void getData();
    void showProducers();
    void setProducerIP();
    void controlTimerStart();
    void controlTimerStop();
    void timerEvent(QTimerEvent *timer);

private:
    unsigned int temp;
    Ui::MainWindow *ui;
    QTcpSocket *socket;
    QString selectedProducerIP;
};

#endif // MAINWINDOW_H
