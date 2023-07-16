#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    socket = new QTcpSocket(this);

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

    connect(ui->pushButtonUpdate,
            SIGNAL(clicked(bool)),
            this,
            SLOT(showProducers())
            );

    connect(ui->pushButtonStart,
            SIGNAL(clicked(bool)),
            this,
            SLOT(controlTimerStart())
            );

    connect(ui->pushButtonStop,
            SIGNAL(clicked(bool)),
            this,
            SLOT(controlTimerStop())
            );
}

void MainWindow::tcpConnect()
{
    QString ip = ui->lineEditIP->text();

    socket->connectToHost(ip,1234);

    if(!(socket->waitForConnected(3000)))
    {
        ui->labelStatus->setText("Erro ao conectar");
        return;
    }
    ui->labelStatus->setText("Conectado");
}

void MainWindow::tcpDisconnect()
{
    if(!(socket->state() == QAbstractSocket::ConnectedState))
    {
        return;
    }
    socket->disconnectFromHost();
    ui->labelStatus->setText("Desconectado");
}

void MainWindow::showProducers()
{
    QString str;

    if(socket->state() != QAbstractSocket::ConnectedState)
    {
        return;
    }

    ui->listWidgetIP->clear();

    socket->write("list\r\n");
    socket->waitForBytesWritten();
    socket->waitForReadyRead();

    while(socket->bytesAvailable())
    {
        str = socket->readLine().replace("\n","").replace("\r","");
        ui->listWidgetIP->addItem(str);

    }
}

void MainWindow::setProducerIP()
{
    if(socket->state() != QAbstractSocket::ConnectedState)
    {
        return;
    }

    this->selectedProducerIP = ui->lineEditIP->selectedText();
}

void MainWindow::getData()
{
    QString str, get_command;
    QByteArray aux;

    get_command = "get " + ui->listWidgetIP->currentItem()->text() + " 1\r\n";
    aux = get_command.toLatin1();

    socket->write(aux);
    socket->waitForBytesWritten();
    socket->waitForReadyRead();

    if (socket->bytesAvailable())
    {
        str = socket->readLine().replace("\n","").replace("\r","");
    }
}

void MainWindow::timerEvent(QTimerEvent *timer)
{
    getData();
}

void MainWindow::controlTimerStart()
{
    temp = startTimer(ui->horizontalSliderTiming->value() * 1000);
}

void MainWindow::controlTimerStop()
{
    killTimer(temp);
}

MainWindow::~MainWindow()
{
    delete socket;
    delete ui;
}
