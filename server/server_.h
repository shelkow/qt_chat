#ifndef SERVER__H
#define SERVER__H
#include <QTcpServer>
#include <QTcpSocket>
#include <QVector>
#include <QTime>
//Сервер создает новый сокет для каждого нового подключения
//Создадим класс, который наследуется от QTcpServer
class Server:public QTcpServer
{
    Q_OBJECT //класс для слот-сигнальной модели
public:
    Server(); //конструктор класса
    QTcpSocket *socket;//создаем объект класса QTcpSocket
private:
    QVector <QTcpSocket*> Sockets;//вектор для сокетов
    QByteArray Data;//массив байт
    QString nick_name;
    void SendToClient(QString str);//функция для отправки данных клиенту
    qint16 nextBlockSize;
public slots:
    void incomingConnection(qintptr socketDescriptor);//обработчик новых подключений
    void slotDisConnect();//обработчик новых подключений
    void slotReadyRead();//обработчик полученных от клиента сообщений
};

#endif // SERVER__H
