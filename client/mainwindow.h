#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QTime>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_lineEdit_returnPressed();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::MainWindow *ui;
    QTcpSocket *socket;
    QByteArray Data;
    QString nick_name;
    void SendToServer(QString str);
    //переменная для хранения размера блока (целое положительное число) 16бит=2байта
    qint16 nextBlockSize;
public slots:
    void slotReadyRead();
    void slotDisConnect();


};
#endif // MAINWINDOW_H
