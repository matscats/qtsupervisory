#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent), ui(new Ui::MainWindow){
  ui->setupUi(this);
  socket = new QTcpSocket(this);

  connect(ui->pushButtonStart,
          SIGNAL(clicked(bool)),
          this,
          SLOT(putData())
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
}

void MainWindow::tcpConnect(){
  socket->connectToHost("127.0.0.1",1234);
  if(socket->waitForConnected(3000)){
    qDebug() << "Connected";
  }
  else{
    qDebug() << "There was an error in connecting to host";
  }
}

void MainWindow::tcpDisconnect()
{
  if(socket->state() == QAbstractSocket::ConnectedState)
  {
    socket->disconnectFromHost();
    qDebug() << "Disconnected from host";
  }
  else
  {
    qDebug() << "You are not connected to any host";
  }
}

void MainWindow::putData(){

  QDateTime datetime;
  QString str;
  qint64 msecdate;

  if(socket->state()== QAbstractSocket::ConnectedState){

    msecdate = QDateTime::currentDateTime().toMSecsSinceEpoch();
    str = "set "+ QString::number(msecdate) + " " +
        QString::number(rand()%35)+"\r\n";

      qDebug() << str;
      qDebug() << socket->write(str.toStdString().c_str())
               << " bytes written";
      if(socket->waitForBytesWritten(3000)){
        qDebug() << "wrote";
      }
  }
}

MainWindow::~MainWindow(){
  delete socket;
  delete ui;
}
