#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    socket = new QTcpSocket(this);

    connect(ui->pushButtonStart,
            SIGNAL(clicked(bool)),
            this,
            SLOT(controlTimerStart())
            );

    connect(ui->pushButtonConnect,
            SIGNAL(clicked(bool)),
            this,
            SLOT(tcpConnect())
            );

    connect(ui->pushButtonDisconnect,
            SIGNAL(clicked(bool)),
            this,
            SLOT(tcpDisconnect())
            );

    connect(ui->pushButtonStop,
               SIGNAL(clicked(bool)),
               this,
               SLOT(controlTimerStop())
               );
}

void MainWindow::tcpConnect()
{
    socket->connectToHost("127.0.0.1",1234);

    if(!(socket->waitForConnected(3000)))
    {
        qDebug() << "There was an error in connecting to host";
        return;
    }
    qDebug() << "Connected";
}

void MainWindow::tcpDisconnect()
{
    if(!(socket->state() == QAbstractSocket::ConnectedState))
    {
        qDebug() << "You are not connected to any host";
        return;
    }
    socket->disconnectFromHost();
    qDebug() << "Disconnected from host";
}

void MainWindow::putData()
{
    QString str;
    qint64 msecdate;

    int min = ui->horizontalSliderMin->value();
    int max = ui->horizontalSliderMax->value();
    int aleat = min + std::rand() % (max - min + 1);

    if(socket->state()== QAbstractSocket::ConnectedState)
    {
        msecdate = QDateTime::currentDateTime().toMSecsSinceEpoch();
        str = "set "+ QString::number(msecdate) + " " +
              QString::number(aleat)+"\r\n";
        qDebug() << str;
        qDebug() << socket->write(str.toStdString().c_str())
                 << " bytes written";
        if(socket->waitForBytesWritten(3000))
        {
            qDebug() << "wrote";
        }
        ui->textBrowser->append(str);
    }
}

void MainWindow::controlTimerStart()
{
    QString str;
    str = "Sending data";
    ui->textBrowser->append(str);
    temp = startTimer(ui->horizontalSliderTime->value() * 1000);
}

void MainWindow::controlTimerStop()
{
    QString str;
    str = "Stop sending data";
    ui->textBrowser->append(str);
    killTimer(temp);
}

void MainWindow::timerEvent(QTimerEvent *timer)
{
    putData();
}

MainWindow::~MainWindow()
{
    delete socket;
    delete ui;
}
