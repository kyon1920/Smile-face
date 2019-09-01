#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QHostAddress>
#include <QTcpServer>    // 监听套接字
#include <QTcpSocket>    // 通信套接字
#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QPixmap>
#include <QFile>
#include <QByteArray>
#include <QString>
#include <QStringList>
#include <QIcon>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();
    void sqlOperator();
    void sqlOper0(QStringList srlist, QTcpSocket *tcpSocket);
    void sqlOper1(QStringList srlist, QTcpSocket *tcpSocket);
    void sqlOper2(QStringList srlist, QTcpSocket *tcpSocket);
    void sqlOper3(QStringList srlist, QTcpSocket *tcpSocket);
    void sqlOper4(QStringList srlist, QTcpSocket *tcpSocket);
    void sqlOper5(QStringList srlist, QTcpSocket *tcpSocket);
    void sqlOper6(QStringList srlist, QTcpSocket *tcpSocket);
    void sqlOper7(QStringList srlist, QTcpSocket *tcpSocket);
    void sqlOper8(QStringList srlist, QTcpSocket *tcpSocket);
    void sqlOper9(QStringList srlist, QTcpSocket *tcpSocket);
    void sqlOper10(QStringList srlist, QTcpSocket *tcpSocket);
    void sqlOper11(QStringList srlist, QTcpSocket *tcpSocket);

private:
    Ui::Widget *ui;
    QTcpServer *tcpServer;
    QTcpSocket *tcpSocket;
    QSqlDatabase database;
};

#endif // WIDGET_H
