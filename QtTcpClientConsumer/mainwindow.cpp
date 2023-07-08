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
            SLOT(getData())
            );

    connect(ui->pushButtonUpdate,
            SIGNAL(clicked(bool)),
            this,
            SLOT(showProducers())
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
    if(socket->state() != QAbstractSocket::ConnectedState)
    {
        return;
    }

    if (socket->state() != QAbstractSocket::ListeningState)
    {
        return;
    }


    ui->listWidgetIP->clear();

    QList<QTcpSocket*> connectedProducers = socket->findChildren<QTcpSocket*>();

    for (QTcpSocket* producer : connectedProducers) {
        QString producer_ip = producer->peerAddress().toString();
        ui->listWidgetIP->addItem(producer_ip);
    }
}

//void MainWindow::setProducerIP()
//{
//    if(!(socket->state() == QAbstractSocket::ConnectedState))
//    {
//        return;
//    }
//    ui->lineEditIP->text()
//}

void MainWindow::getData(){
    QString str;
    QByteArray array;
    QStringList list;
    qint64 thetime;
    qDebug() << "to get data...";
    if(socket->state() == QAbstractSocket::ConnectedState){
        if(socket->isOpen()){
            qDebug() << "reading...";
            socket->write("get 127.0.0.1 5\r\n");
            socket->waitForBytesWritten();
            socket->waitForReadyRead();
            qDebug() << socket->bytesAvailable();
            while(socket->bytesAvailable()){
                str = socket->readLine().replace("\n","").replace("\r","");
                list = str.split(" ");
                if(list.size() == 2){
                    bool ok;
                    str = list.at(0);
                    thetime = str.toLongLong(&ok);
                    str = list.at(1);
                    qDebug() << thetime << ": " << str;
                }
            }
        }
    }
}


MainWindow::~MainWindow()
{
    delete socket;
    delete ui;
}
