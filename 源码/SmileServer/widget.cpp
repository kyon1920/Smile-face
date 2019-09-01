#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    sqlOperator();
    tcpServer = nullptr;
    tcpSocket = nullptr;
    this -> setWindowTitle("服务器(端口:8888)");
    this -> setWindowIcon(QIcon("://icon.jpg"));
    this -> resize(450,350);
    this -> move(500,300);
    // 指定父对象，让它自动回收空间
    tcpServer = new QTcpServer(this);    // 监听套接字
    // 监听 设定地址和端口
    tcpServer -> listen(QHostAddress::Any, 8888);
    connect(tcpServer, &QTcpServer::newConnection,
            [=](){
                     // 取出建立好链接的套接字
                     tcpSocket = tcpServer -> nextPendingConnection();
                     // 获取客户端的IP和端口
                     QString ip = tcpSocket -> peerAddress().toString();
                     qint16 port = tcpSocket -> peerPort();
                     QString temp = QString("[%1:%2]:成功连接").arg(ip).arg(port);
                     ui -> textEdit -> append(temp);
                     connect(tcpSocket, &QTcpSocket::readyRead,
                                             [=]()
                                                 {
                                                     // 从通信套接字中取出内容
                                                     QByteArray array = tcpSocket -> readAll();
                                                     ui -> textEdit -> append("客户端:" + array);
                                                     QString sr = array;
                                                     QStringList srlist = sr.split('|');
                                                     switch (srlist.at(0).toInt())
                                                     {
                                                     case 0:
                                                         // 注册
                                                         Widget::sqlOper0(srlist, tcpSocket);
                                                         break;
                                                     case 1:
                                                         // 登录
                                                         Widget::sqlOper1(srlist, tcpSocket);
                                                         break;
                                                     case 2:
                                                         // 添加好友
                                                         Widget::sqlOper2(srlist, tcpSocket);
                                                         break;
                                                     case 3:
                                                         // 列表
                                                         Widget::sqlOper3(srlist, tcpSocket);
                                                         break;
                                                     case 4:
                                                         // 判断是否在线
                                                         Widget::sqlOper4(srlist, tcpSocket);
                                                         break;
                                                     case 5:
                                                         // 离线
                                                         Widget::sqlOper5(srlist, tcpSocket);
                                                         break;
                                                         // 查看事务
                                                     case 6:
                                                         Widget::sqlOper6(srlist, tcpSocket);
                                                         break;
                                                         // 添加事务
                                                     case 7:
                                                         Widget::sqlOper7(srlist, tcpSocket);
                                                         break;
                                                         // 删除事务
                                                     case 8:
                                                         Widget::sqlOper8(srlist, tcpSocket);
                                                         break;
                                                         // 获取人生日历
                                                     case 9:
                                                         Widget::sqlOper9(srlist, tcpSocket);
                                                         break;
                                                         // IM端口
                                                     case 10:
                                                         Widget::sqlOper10(srlist, tcpSocket);
                                                         break;
                                                         // 删除好友
                                                     case 11:
                                                         Widget::sqlOper11(srlist, tcpSocket);
                                                         break;
                                                     }
                                                 });
      });
}

void Widget::sqlOperator()
{
    //打开数据库
    database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(".//smiledb.db");
    if (!database.open())
    {
        qDebug() << "Error: Failed to connect database." << database.lastError();
    }
    else
    {
        qDebug() << "Succeed to connect database." ;
    }
    //创建表格
    QSqlQuery sql_query;
    if(!sql_query.exec("create table logon(username text primary key, password text, year int, month int, day int, isonline int)"))
    {
        qDebug() << "Error: Fail to create table." << sql_query.lastError();
    }
    else
    {
        qDebug() << "Table created!";
    }

    /*//插入数据
    if(!sql_query.exec("INSERT INTO logon VALUES('oubc', 'oubc', 2000, 1, 1, 0)"))
    {
        qDebug() << sql_query.lastError();
    }
    else
    {
        qDebug() << "inserted Success!";
    }*/

    /*QByteArray ba1;
    QPixmap p1("://h1.jpg");
    QDataStream ds1(&ba1, QIODevice::WriteOnly);
    ds1 << p1;*/
    /*if(!sql_query.exec("INSERT INTO logon VALUES('zhangshao', 'zhangshao', 2000, 1, 1, 0)"))
    {
        qDebug() << sql_query.lastError();
    }
    else
    {
        qDebug() << "inserted Success!";
    }*/
    /*sql_query.exec("update logon set headpic=':inByteArray' where username='zhangshao'");
    sql_query.bindValue( ":inByteArray", inByteArray);*/
    //修改数据
    /*sql_query.exec("update logon set name = \"QT\" where id = 1");
    if(!sql_query.exec())
    {
        qDebug() << sql_query.lastError();
    }
    else
    {
        qDebug() << "updated!";
    }*/

    //查询数据
    sql_query.exec("select * from logon");
    if(!sql_query.exec())
    {
        qDebug()<<sql_query.lastError();
    }
    else
    {
        while(sql_query.next())
        {
            //QString username = sql_query.value(0).toString();
            //QString password = sql_query.value(1).toString();
            //int year = sql_query.value(2).toInt();
            //int month = sql_query.value(3).toInt();
            //int day = sql_query.value(4).toInt();
            //qDebug() << QString("username:%1  password:%2  year:%3  month:%4  day:%5").arg(username).arg(password).arg(year).arg(month).arg(day);
            QByteArray outByteArray = sql_query.value(5).toByteArray();
            QPixmap outPixmap = QPixmap();
            outPixmap.loadFromData(outByteArray);

            /*ui -> label -> setPixmap(outPixmap);
            ui -> label -> setScaledContents(true);*/
        }
    }

    //删除数据
    /*sql_query.exec("delete from logon where month = 1");
    if(!sql_query.exec())
    {
        qDebug()<<sql_query.lastError();
    }
    else
    {
        qDebug()<<"deleted!";
    }*/

    //删除表格
    /*sql_query.exec("drop table logon");
    if(sql_query.exec())
    {
        qDebug() << sql_query.lastError();
    }
    else
    {
        qDebug() << "table cleared";
    }*/

    // 创建聊天端口数据库
    QSqlQuery sql_port;
    if(!sql_port.exec("create table port(username text, port INTEGER PRIMARY KEY AUTOINCREMENT)"))
    {
         qDebug() << "Error: Fail to create table(port)." << sql_port.lastError();
    }
    else
    {
        qDebug() << "Table created(port)!";
    }

}

void Widget::sqlOper0(QStringList srlist, QTcpSocket *tcpSocket)
{
    QString username = srlist.at(1);
    QString password = srlist.at(2);
    int year = srlist.at(3).toInt();
    int month = srlist.at(4).toInt();
    int day = srlist.at(5).toInt();

    QSqlQuery sql_port;
    if(!sql_port.exec(QString("insert into port(username) values('%1')").arg(username)))
    {
        //qDebug() << "Error: Fail to table insert." << sql_create.lastError();
        ui -> textEdit -> append("port添加失败");
        ui -> textEdit -> append(QString("insert into port(username) values('%1')").arg(username));
    }
    else
    {
        ui -> textEdit -> append("port添加成功");
   }

    QSqlQuery sql_query;
    QString insert_sql = "insert into logon values (?, ?, ?, ?, ?, 0)";
    sql_query.prepare(insert_sql);
    sql_query.addBindValue(username);
    sql_query.addBindValue(password);
    sql_query.addBindValue(year);
    sql_query.addBindValue(month);
    sql_query.addBindValue(day);
    if(!sql_query.exec())
    {
        ui -> textEdit -> append("用户注册失败");
        QByteArray error = "error";
        tcpSocket -> write(error);
    }
    else
    {
        ui -> textEdit -> append("用户注册成功");
        QByteArray error = "success";
        tcpSocket -> write(error);

        /*//打开数据库
        database = QSqlDatabase::addDatabase("QSQLITE");
        database.setDatabaseName("A:/Smile-face/SmileServer/db/smiledb.db");*/
        //创建表格
        QSqlQuery sql_create;
        if(!sql_create.exec(QString("create table x%1(username text primary key, year int)").arg(username)))
        {
            qDebug() << "Error: Fail to create table for x." << sql_create.lastError();
        }
        else
        {
            qDebug() << "Table created!";
        }

        QSqlQuery sql_event;
        if(!sql_event.exec(QString("create table e%1(date text, event text primary key)").arg(username)))
        {
            qDebug() << "Error: Fail to create table for event." << sql_create.lastError();
        }
        else
        {
            qDebug() << "Table created!";
        }
    }
}

void Widget::sqlOper1(QStringList srlist, QTcpSocket *tcpSocket)
{
    int su = 0;
    QString username = srlist.at(1);
    QString password = srlist.at(2);
    QString password1;
    QSqlQuery sql_query;
    QString sqlquery = "select password from logon where username = ?";
    sql_query.prepare(sqlquery);
    sql_query.addBindValue(username);
    if(!sql_query.exec())
    {
        qDebug()<<sql_query.lastError();
    }
    else
    {
        sql_query.next();
        password1 = sql_query.value(0).toString();
        if (password1 == password)
        {
            su = 1;
        }
    }
    if(su == 0)
    {
        ui -> textEdit -> append("用户登录失败");
        QByteArray error = "error";
        tcpSocket -> write(error);
    }
    else
    {
        ui -> textEdit -> append("用户登录成功");
        QByteArray error = "success";
        tcpSocket -> write(error);

        QSqlQuery sql_update;
        if(!sql_update.exec(QString("update logon set isonline = 1 where username = %1").arg(username)))
        {
            qDebug()<<sql_query.lastError();
        }
        else
        {
            ui -> textEdit -> append("数据更新成功！");
        }
    }
}

void Widget::sqlOper2(QStringList srlist, QTcpSocket *tcpSocket)
{
    int su = 0;
    QString username = srlist.at(1);
    QString addUsername = srlist.at(2);
    ui -> textEdit -> append(username);
    ui -> textEdit -> append(addUsername);
    QString addUsername1;
    int year;
    QSqlQuery sql_query;
    QString sqlquery = "select username from logon where username = ?";
    sql_query.prepare(sqlquery);
    sql_query.addBindValue(addUsername);
    if(!sql_query.exec())
    {
        qDebug()<<sql_query.lastError();
    }
    else
    {
        sql_query.next();
        addUsername1 = sql_query.value(0).toString();
        if (addUsername == addUsername1)
        {
            su = 1;
        }
    }
    QString sqlquery1 = "select year from logon where username = ?";
    sql_query.prepare(sqlquery1);
    sql_query.addBindValue(addUsername);
    if(!sql_query.exec())
    {
        qDebug()<<sql_query.lastError();
    }
    else
    {
        sql_query.next();
        year = sql_query.value(0).toInt();
    }
    if(su == 0)
    {
        ui -> textEdit -> append("添加好友失败");
        QByteArray error = "error";
        tcpSocket -> write(error);
    }
    else
    {
        QSqlQuery sql_create;
        if(!sql_create.exec(QString("insert into x%1 values(%2, %3)").arg(username).arg(addUsername).arg(year)))
        {
            //qDebug() << "Error: Fail to table insert." << sql_create.lastError();
            ui -> textEdit -> append("好友添加失败");
            ui -> textEdit -> append(QString("insert into x%1 values('%2', %3)").arg(username).arg(addUsername).arg(year));
            ui -> textEdit -> append(addUsername1);
            ui -> textEdit -> append(QString::number(year));
            QByteArray error = "error";
            tcpSocket -> write(error);
        }
        else
        {
            ui -> textEdit -> append("好友添加成功");
            QByteArray error = "success";
            tcpSocket -> write(error);
        }
    }
}

void Widget::sqlOper3(QStringList srlist, QTcpSocket *tcpSocket)
{
    QString username = srlist.at(1);
    QSqlQuery sql_query;
    QString count_sql =  QString("select count(username) from x%1").arg(username);
    //"insert into logon values (?, ?, ?, ?, ?, 0)";
    if(!sql_query.exec(count_sql))
    {
        ui -> textEdit -> append("好友加载失败");
        QByteArray error = "error";
        tcpSocket -> write(error);
        QString str = QString("select count(*) from x%1").arg(username);
        ui -> textEdit -> append(str);
        sql_query.next();
        int num = sql_query.value(0).toInt();
        ui -> textEdit -> append(QString::number(num));
    }
    else
    {
        sql_query.next();
        int num = sql_query.value(0).toInt();
        QString mess = QString::number(num) + "|";
        QString c_sql =  QString("select * from x%1").arg(username);
        if(!sql_query.exec(c_sql))
        {
            ui -> textEdit -> append("加载好友失败");
            QByteArray error = "error";
            tcpSocket -> write(error);
        }
        else
        {
            for (int i =0; i < num; ++i)
            {
                sql_query.next();
                QString str = sql_query.value(0).toString();
                int ye = sql_query.value(1).toInt();
                mess += (str + "|" + QString::number(ye) +"|");
            }
            tcpSocket -> write(mess.toLatin1());
            ui -> textEdit -> append("好友加载成功");
            ui -> textEdit -> append(mess);
        }
    }
}

void Widget::sqlOper4(QStringList srlist, QTcpSocket *tcpSocket)
{
    QString username = srlist.at(1);
    QSqlQuery sql_query;
    QString count_sql =  QString("select isonline from logon where username=%1").arg(username);
    if(!sql_query.exec(count_sql))
    {
        ui -> textEdit -> append("判断在线状态失败!");
    }
    else
    {
        sql_query.next();
        int num = sql_query.value(0).toInt();
        ui -> textEdit -> append(QString::number(num) + "好友在线状态判断成功!");
        if (num == 0)
        {
            QByteArray error = "error";
            tcpSocket -> write(error);
        }
        else
        {
            QByteArray success = "success";
            tcpSocket -> write(success);
        }
    }
}

void Widget::sqlOper5(QStringList srlist, QTcpSocket *tcpSocket)
{
    QString username = srlist.at(1);
    QSqlQuery sql_update;
    QString update_sql =  QString("update logon set isonline = 0 where username = %1").arg(username);
    if(!sql_update.exec(update_sql))
    {
        qDebug()<<sql_update.lastError();
        ui -> textEdit -> append("数据更新失败！");
    }
    else
    {
        ui -> textEdit -> append("数据更新成功！");
    }
}

void Widget::sqlOper6(QStringList srlist, QTcpSocket *tcpSocket)
{
    QString username = srlist.at(1);
    QString date = srlist.at(2);
    QSqlQuery sql_query;
    QString count_sql =  QString("select event from e%1 where date=%2").arg(username).arg(date);
    if(!sql_query.exec(count_sql))
    {
        ui -> textEdit -> append("查看事务失败!");
        QByteArray error = "error";
        tcpSocket -> write(error);
    }
    else
    {
        sql_query.next();
        QString event = sql_query.value(0).toString();
        if (event == nullptr)
        {
            ui -> textEdit -> append(event + "  事务查看成功!");
            QByteArray success = "success";
            tcpSocket -> write(success);
        }
        else
        {
            ui -> textEdit -> append(event + "  事务查看成功!");
            tcpSocket -> write(event.toLatin1());
        }
    }
}

void Widget::sqlOper7(QStringList srlist, QTcpSocket *tcpSocket)
{
    QString username = srlist.at(1);
    QString date = srlist.at(2);
    QString event = srlist.at(3);
    ui -> textEdit -> append(username);
    ui -> textEdit -> append(date);
    ui -> textEdit -> append(event);
    QSqlQuery sql_create;
    if(!sql_create.exec(QString("insert into e%1 values('%2', '%3')").arg(username).arg(date).arg(event)))
    {
        //qDebug() << "Error: Fail to table insert." << sql_create.lastError();
        ui -> textEdit -> append("事务添加失败");
        ui -> textEdit -> append(QString("insert into e%1 values('%2', '%3')").arg(username).arg(date).arg(event));
        QByteArray error = "error";
        tcpSocket -> write(error);
    }
    else
    {
        ui -> textEdit -> append("事务添加成功");
        QByteArray error = "success";
        tcpSocket -> write(error);
   }
}

void Widget::sqlOper8(QStringList srlist, QTcpSocket *tcpSocket)
{
    QString username = srlist.at(1);
    QString date = srlist.at(2);
    ui -> textEdit -> append(username);
    ui -> textEdit -> append(date);
    QSqlQuery sql_create;
    if(!sql_create.exec(QString("delete from e%1 where date=%2").arg(username).arg(date)))
    {
        //qDebug() << "Error: Fail to table insert." << sql_create.lastError();
        ui -> textEdit -> append("事务删除失败");
        ui -> textEdit -> append(QString("delete from e%1 where date=%2").arg(username).arg(date));
        QByteArray error = "error";
        tcpSocket -> write(error);
    }
    else
    {
        ui -> textEdit -> append("事务删除成功");
        QByteArray error = "success";
        tcpSocket -> write(error);
   }
}

void Widget::sqlOper9(QStringList srlist, QTcpSocket *tcpSocket)
{
    QString username = srlist.at(1);
    ui -> textEdit -> append(username);
    QSqlQuery sql_query;
    if(!sql_query.exec(QString("select year, month, day from logon where username=%1").arg(username)))
    {
        ui -> textEdit -> append("人生日历获取失败");
        QByteArray error = "error";
        tcpSocket -> write(error);
    }
    else
    {
        sql_query.next();
        int ye = sql_query.value(0).toInt();
        int mo = sql_query.value(1).toInt();
        int da = sql_query.value(2).toInt();
        int port;

        QSqlQuery sql_port;
        if(!sql_port.exec(QString("select port from port where username=%1").arg(username)))
        {
            ui -> textEdit -> append("port获取失败");
            QByteArray error = "error";
            tcpSocket -> write(error);
        }
        else
        {
            sql_port.next();
            port = sql_port.value(0).toInt();
            QString mess = QString::number(ye) + "|" + QString::number(mo) +"|" + QString::number(da) + "|" + QString::number(port) + "|";
            tcpSocket -> write(mess.toLatin1());
            ui -> textEdit -> append("人生日历|port获取成功" + mess);
        }
   }
}

void Widget::sqlOper10(QStringList srlist, QTcpSocket *tcpSocket)
{
    QString username = srlist.at(1);
    ui -> textEdit -> append(username);
    QSqlQuery sql_query;
    if(!sql_query.exec(QString("select port from port where username=%1").arg(username)))
    {
        ui -> textEdit -> append("port获取失败");
        QByteArray error = "error";
        tcpSocket -> write(error);
    }
    else
    {
        sql_query.next();
        int port = sql_query.value(0).toInt();
        QString mess = QString::number(port);
        tcpSocket -> write(mess.toLatin1());
        ui -> textEdit -> append("port获取成功" + mess);
   }
}

void Widget::sqlOper11(QStringList srlist, QTcpSocket *tcpSocket)
{
    QString username = srlist.at(1);
    QString deluser = srlist.at(2);
    ui -> textEdit -> append(username + "删除" + deluser);

    QSqlQuery sql_del;
    if(!sql_del.exec(QString("delete from x%1 where username=%2").arg(username).arg(deluser)))
    {
        ui -> textEdit -> append("删除好友失败");
        QByteArray error = "error";
        tcpSocket -> write(error);
    }
    else
    {
        QByteArray success = "success";
        tcpSocket -> write(success);
        ui -> textEdit -> append("删除好友成功" );
   }
}

Widget::~Widget()
{
    //关闭数据库
    database.close();
    delete ui;
}
