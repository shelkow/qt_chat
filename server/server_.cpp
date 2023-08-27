#include <server_.h>
Server::Server()
{
    //QHostAddress - принимает сигналы с любого адреса, сервер прослушивает порт 2323
    if(this->listen(QHostAddress::Any,2323))
    {
        qDebug()<<"start";

    }
    else
    {
        qDebug()<<"error";
    }
    nextBlockSize=0;

}
void Server::incomingConnection(qintptr socketDescriptor)
{

    socket=new QTcpSocket;//Создаем сокет
    socket->setSocketDescriptor(socketDescriptor);//Descriptor идентифицирует поток ввода/вывода (неотрицательное число)
    connect(socket,&QTcpSocket::readyRead,this,&Server::slotReadyRead);// объединяем сигнал со слотом
    connect(socket,&QTcpSocket::disconnected,this,&Server::slotDisConnect);//объединяем сигнал disconnected с функцией deletelater

    Sockets.push_back(socket);//поместим сокет в контейнер
    qDebug()<<"client connected"<<socketDescriptor;
}

void Server::slotDisConnect()
{
    //при отключении клиента будет происходит удаление сокета
    qDebug() << "client disconnected";
    socket->deleteLater();
    Sockets.removeFirst();
}

void Server::slotReadyRead()
{
    socket = (QTcpSocket*)sender();//записываем сокет, с которого пришел запрос
    QDataStream in(socket);//c помощью объекта in работаем с данными находящимися в сокете
    in.setVersion(QDataStream::Qt_6_2); //устанавливаем версию
//Каждый символ QString занимает два байта
    if (in.status()==QDataStream::Ok)
    {
        for(;;)//бесконечный цикл
        {
            //считываем размер блока
            if(nextBlockSize == 0) //если размер блока неизвестен
            {
                //qDebug()<<"BlockSize = 0";
                if (socket->bytesAvailable()<2) //для чтения доступно не меньше двух байт
                {
                    qDebug()<<"Data <2,error";
                    break;//выходим из цикла
                }
                in >> nextBlockSize;//считываем размер блока
                qDebug()<<"Size of message = "<<nextBlockSize<<"byte";
            }
            //сравниваем размер блока с количеством байт которое пришло от сервера
            if (socket->bytesAvailable()<nextBlockSize)//если размер блока больше то данные пришли не полностью
            {
                qDebug()<<"Data not full, error";
                break;//выходим из цикла
            }

            QString str;
            QTime time;
            in >>time>>str;
            nextBlockSize=0;
            qDebug()<<time.toString()<<str;
            SendToClient(str);
            break;

        }
    }
    else
    {
        qDebug()<<"DataStream error";

    }
  /*  if (in.status() == QDataStream::Ok) //проверяем состояние объекта in
    {
        qDebug()<<"read";
        QString str;
        in>>str; //из объекта in запишем строку
        qDebug()<<str;
        SendToClient(str);
    }
    else
    {
        qDebug()<<"DataStream error";
    }
*/
}

void Server::SendToClient(QString str)
{
    Data.clear(); //чистим массив Data
    QDataStream out(&Data,QIODevice::WriteOnly);//создаем объект out(данные на вывод) &Data - массив байтов, Режим работы WriteOnly
    out.setVersion(QDataStream::Qt_6_2);
    //выделяем два байта вначале сообщения, cледующая часть с 3 байта
    out << qint16(0)<<QTime::currentTime()<<str;//через объект out записываем строку в массив Data
    out.device()->seek(0);//переходим в самое начало блока
    out<<qint16(Data.size()-sizeof(qint16)-8);//Разность между длиной всего блока и выделенными данными для размера
   // socket->write(Data);//записываем массив в сокет
    for (int i=0;i<Sockets.size();i++)
    {
        Sockets[i]->write(Data);
    }
}
