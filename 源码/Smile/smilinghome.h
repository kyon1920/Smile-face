#ifndef SMILINGHOME_H
#define SMILINGHOME_H

#include "addfriend.h"

#include <QWidget>
#include <QTimer>
#include <QTime>
#include <QDateTime>
#include <QPainter>
#include <QMovie>
#include <QDebug>
#include <QToolButton>
#include <QStyle>
#include <QPixmap>
#include <QIcon>
#include <QMouseEvent>
#include <QPushButton>
#include <QBitmap>
#include <QPen>
#include <QBrush>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QMessageBox>
#include <QTcpSocket>
#include <QStringList>
#include <QThread>
#include <QIcon>
#include <QMessageBox>
#include <QTextDocumentFragment>
#include <QFile>
#include <QFileDialog>
#include <QDate>
#include <QUdpSocket>
#include <QHostAddress>
#include <QMap>
#include <QTcpServer>
#include <QFileInfo>

#define cout qDebug() << "[" << __FILE__ << ":" << __LINE__ << "]"

namespace Ui {
class SmilingHome;
}

class SmilingHome : public QWidget
{
    Q_OBJECT

public:
    explicit SmilingHome(QWidget *parent = nullptr);
    ~SmilingHome();
    // 设置窗体函数
    void setWindow();
    void windowclosed();
    void windowmin();
    void mouseMoveEvent(QMouseEvent* mEvent);
    void mousePressEvent(QMouseEvent* mEvent);
    void setTime();
    void changeUser();
    void listFriends();
    void listDClicked(QTreeWidgetItem *item, int column);
    void listClicked(QTreeWidgetItem *item, int column);
    void TikTok();
    void setGit();
    void setTimers();
    void udpRecv();
    void sendData();

signals:
    void changeUsersignal(int, QString);

private slots:
    void on_refresh_clicked();

    void on_addFriends_clicked();

    void on_mainButton_clicked();

    void on_timeButton_clicked();

    void on_headi_clicked();

    void on_noline_clicked();

    void on_addpic_clicked();

    void on_addtime_clicked();

    void on_addevent_clicked();

    void on_pushButton_clicked();

    void on_calendar_clicked(const QDate &date);

    void on_addevents_clicked();

    void on_delevent_clicked();

    void on_recvfile_clicked();

    void on_tiktok_clicked();

    void on_sendfile_clicked();

    void on_cancel_clicked();

    void on_send_clicked();

    void on_delfriend_clicked();

private:
    Ui::SmilingHome *ui;
    QTimer *time;
    QPoint dragPosition;
    QString username;
    addFriend *add;
    QTcpSocket *tSocket;
    int num;    // 好友数量
    QTreeWidgetItem *Friends;
    QTreeWidgetItem *stranger;
    QString sr;
    QStringList srlist;
    int *year;
    int sul;
    QTreeWidgetItem *item;
    QString strr;
    QIcon headpic;
    QString selectDate;
    int birye;
    int birmo;
    int birda;
    int ye = 0;
    int mo = 0;
    int da = 0;
    long long int mi = 0;
    long long int se = 0;
    QTimer *yeTimer;
    QTimer *moTimer;
    QTimer *daTimer;
    QTimer *miTimer;
    QTimer *seTimer;
    QUdpSocket *uSocket;
    QMultiMap<QString, QString> unreadmess;
    int mport;
    int yport;
    QTcpSocket *fSocket;
    QTcpServer *fServer;
    QFile sfile;    // 文件对象
    QString sfileName;    // 文件名字
    qint64 sfileSize;    // 文件大小
    qint64 sendSize;    // 已经发送文件大小
    QTimer ftimer;    // 定时器
    QTcpSocket *rSocket;
    QFile rfile;    // 文件对象
    QString rfileName;    // 文件名字
    qint64 rfileSize;    // 文件大小
    qint64 recvSize;    // 已经发送文件大小
    bool isStart;
};

#endif // SMILINGHOME_H
