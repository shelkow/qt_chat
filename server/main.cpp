#include <QCoreApplication>
#include "server_.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Server s;//создаем экзмепляр класса Server
    return a.exec();
}
