#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //Создаем сокет
    socket=new QTcpSocket(this);
    //Соединяем сигналы и слоты
    connect(socket,&QTcpSocket::readyRead,this, &MainWindow::slotReadyRead);
    connect(socket,&QTcpSocket::readyRead,this, &MainWindow::slotReadyRead);
    nextBlockSize=0;
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{   //активация элементов
    ui->lineEdit->setEnabled(true);
    ui->pushButton_2->setEnabled(true);
    //подключаемся к серверу
     QString ip;
     int port;
     ip = ui->lineEdit_2->text();
     port=ui->lineEdit_3->text().toInt();
     socket->connectToHost(ip,port);
}

void MainWindow::SendToServer(QString str)
{
    Data.clear(); //чистим массив Data
    QDataStream out(&Data,QIODevice::WriteOnly);//создаем объект out(данные на вывод) &Data - массив байтов, Режим работы WriteOnly
    out.setVersion(QDataStream::Qt_6_2);
    //выделяем два байта вначале сообщения, cледующая часть с 3 байта
    out << qint16(0)<<QTime::currentTime()<<str;//через объект out записываем строку в массив Data
    out.device()->seek(0);//переходим в самое начало блока
    //Разность между длиной всего блока и выделенными данными для размера
    out<<qint16(Data.size()-sizeof(qint16)-8);

    socket->write(Data);//записываем массив в сокет
    ui->lineEdit->clear();
}

void MainWindow::slotReadyRead()
{
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_6_2);
    if (in.status()==QDataStream::Ok)
    {
        for(;;)//бесконечный цикл
        {
            //считываем размер блока
            if(nextBlockSize == 0) //если размер блока неизвестен
            {
                if (socket->bytesAvailable()<2) //для чтения доступно не меньше двух байт
                {
                    break;//выходим из цикла
                }
                in >> nextBlockSize;//считываем размер блока
            }
            //сравниваем размер блока с количеством байт которое пришло от сервера
            if (socket->bytesAvailable()<nextBlockSize)//если размер блока больше то данные пришли не полностью
            {
                break;//выходим из цикла
            }
            QTime time;
            QString str;
            in >>time>>str;
            nextBlockSize=0;
            ui->textBrowser->append(time.toString()+" "+str);

        }
    }
    else
    {
        ui->textBrowser->append("read error");

    }
    /* if (in.status()==QDataStream::Ok)
    {
        QString str;
        in>>str;
        ui->textBrowser->append(str);
    }
    else
    {
        ui->textBrowser->append("read error");

    }
*/

}

void MainWindow::slotDisConnect()
{
   socket->deleteLater();
}


void MainWindow::on_pushButton_2_clicked()
{
    SendToServer(ui->lineEdit->text());
}

//отправка сообщения по нажатию на enter
void MainWindow::on_lineEdit_returnPressed()
{
    SendToServer(ui->lineEdit->text());
}


void MainWindow::on_pushButton_3_clicked()
{
    socket->disconnectFromHost();
    //деактивация элементов
    ui->lineEdit->setEnabled(false);
    ui->pushButton_2->setEnabled(false);

}





void MainWindow::on_pushButton_4_clicked()
{
    ui->lineEdit_2->setText("127.0.0.1");
    ui->lineEdit_3->setText("2323");

}


