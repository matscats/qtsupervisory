#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <vector>

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
    QStringList list;
    std::vector<long> time;
    std::vector<int> data;

    if (socket->state() != QAbstractSocket::ConnectedState)
    {
        return;
    }

    get_command = "get " + ui->listWidgetIP->currentItem()->text() + " 4\r\n";
    aux = get_command.toLatin1();

    socket->write(aux);
    socket->waitForBytesWritten();
    socket->waitForReadyRead();

    while (socket->bytesAvailable())
    {
        str = socket->readLine().replace("\n","").replace("\r","");
        list = str.split(" ");

        if (list.size() == 2)
        {
            time.push_back(list.at(0).toLong());
            data.push_back(list.at(1).toInt());
        }
    }

    ui->widget->setData(time, data);
}

void MainWindow::timerEvent(QTimerEvent *timer)
{
    getData();
}

void MainWindow::controlTimerStart()
{
    this->temp = startTimer(ui->horizontalSliderTiming->value() * 1000);
}

void MainWindow::controlTimerStop()
{
    killTimer(this->temp);
}

MainWindow::~MainWindow()
{
    delete socket;
    delete ui;
}
