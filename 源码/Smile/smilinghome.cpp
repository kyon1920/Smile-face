#include "smilinghome.h"
#include "ui_smilinghome.h"

SmilingHome::SmilingHome(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SmilingHome)
{
    ui->setupUi(this);
    setWindow();
    // 设置实时
    time = new QTimer(this);
    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
    connect(time, &QTimer::timeout, this, &SmilingHome::setTime);
    time -> start(1000);
    connect(ui -> changeuser, &QPushButton::clicked, this, &SmilingHome::changeUser);

    // 点击左边栏按钮实现页面切换
    ui -> page -> setCurrentIndex(0);
    connect(ui -> timeButton, &QPushButton::released,this,[=](){ui -> page -> setCurrentIndex(0);});
    // connect(ui -> mainButton, &QPushButton::released,this,[=](){ui -> page -> setCurrentIndex(1);});
    connect(ui -> manyButton, &QPushButton::released,this,[=](){ui -> page -> setCurrentIndex(2);});
    connect(ui -> moreButton, &QPushButton::released,this,[=](){ui -> page -> setCurrentIndex(3);});

    connect(ui -> list, &QTreeWidget::itemDoubleClicked, this, &SmilingHome::listDClicked);
    //connect(ui -> list, &QTreeWidget::itemClicked, this, &SmilingHome::listClicked);

    Friends=new QTreeWidgetItem(ui -> list ,QStringList(QString("Friends")));
    stranger=new QTreeWidgetItem(ui -> list);
    stranger->setText(0,"Stranger");
    ui -> send -> hide();
    ui -> sendfile -> hide();
    ui -> cancel -> hide();
    ui -> tiktok -> hide();
    ui -> message -> hide();
    ui -> sendmessage -> hide();
    ui -> xxx -> hide();
    ui -> headi -> hide();
    ui -> delfriend -> hide();

    ui -> userxxx -> hide();
    ui -> headxxx -> hide();
    ui -> textEdit -> hide();
    ui -> label_5 -> hide();

    ui -> progressBar -> hide();
    setGit();

    // 网络部分
    fServer = new QTcpServer(this);
    // 绑定监听
    fServer -> listen(QHostAddress::Any, 6666);
    ui -> message -> append("文件: 等待对方连接...");
    // 如果客户端和服务器连接成功
    // tcpServer会自动触发newConnection()
    connect(fServer, &QTcpServer::newConnection,
                [=]()
                    {
                        // 取出建立好链接的套接字
                        fSocket = fServer -> nextPendingConnection();
                        QString ip = fSocket -> peerAddress().toString();
                        quint16 port = fSocket -> peerPort();
                        // QString str = QString("[%1:%2] 成功连接").arg(ip).arg(port);
                        QString str = "对方已就绪~";
                        ui -> progressBar -> show();
                        ui -> message -> append(str);

                        connect(fSocket, &QTcpSocket::readyRead,
                                [=]()
                                    {
                                        // 取客户端的信息
                                        QByteArray buf = fSocket -> readAll();
                                        QByteArray buf1 = "done";
                                        if (buf == buf1)
                                        {
                                            // 文件接收完毕
                                            // ui -> message -> append("文件: 文件发送完毕!");
                                            ui -> progressBar -> hide();
                                            //sfile.close();
                                            // 断开客户端端口
                                            fSocket -> disconnectFromHost();
                                            fSocket -> close();
                                        }
                                        else
                                        {
                                            // ui -> message -> append("文件: 正在发送...已完成" + buf);
                                        }
                                    });
                    });
    connect(&ftimer, &QTimer::timeout,
                [=]()
                    {
                        // 关闭定时器
                        ftimer.stop();
                        // 发送文件
                        sendData();
                    });

    isStart = true;
    rSocket = new QTcpSocket(this);
    connect(rSocket, &QTcpSocket::readyRead,
                [=]()
                    {
                        // 取出接收的内容
                        QByteArray buf = rSocket -> readAll();
                        qint64 len = rfile.write(buf);
                        recvSize += len;    // 累计接收大小
                        if (isStart == true)
                        {
                            // ui -> message -> append("文件: 正在接受文件头信息...");
                            // 接收头
                            isStart = false;
                            // 解析头部信息 buf = "xxx##1024"
                            // QString str = "hello##1024";
                            // 拆包
                            // str.section("##", 0, 0);
                            // 初始化
                            ui -> message -> append("文件头信息接受完成!");
                            // ui -> message -> append("文件: 正在初始化文件信息到本地...");
                            rfileName = QString(buf).section("##", 0, 0);
                            rfileSize = QString(buf).section("##", 1, 1).toInt();
                            recvSize = 0;
                            ui -> message -> append("文件初始化完成!");
                            // 打开文件
                            rfile.setFileName(rfileName);
                            bool isOk = rfile.open(QIODevice::WriteOnly);
                            if (false == isOk)
                            {
                                qDebug() << "WriteOnly error!";
                                ui -> message -> append("文件: 打开文件失败!");

                                rSocket -> disconnectFromHost();
                                rSocket -> close();
                                return;    // 如果打开失败，中断函数
                            }
                            // ui -> message -> append("文件: 打开文件成功!");

                            // 弹出对话框，显示接收文件的信息
                            QString str = QString("接收的文件: [%1:%2KB]").arg(rfileName).arg(rfileSize);
                            QMessageBox::information(this, "文件信息", str);

                            // 设置进度条
                            ui -> progressBar -> setMinimum(0);    // 最小值
                            ui -> progressBar -> setMaximum(rfileSize/1024);    // 最大值
                            ui -> progressBar -> setValue(0);    // 当前值
                        }
                        else
                        {
                            if (len > 0)
                            {
                                // QString str1 = QString("正在接收...已完成[%1]").arg(recvSize);
                                // ui -> message -> append(str1);
                                // QString str2 = QString::number(recvSize);
                                // rSocket -> write(str2.toUtf8().data());
                            }

                            // 更新进度条
                            ui -> progressBar -> setValue(recvSize/1024);

                            // 文件接收完毕
                            if (recvSize == rfileSize)
                            {
                                // QByteArray buf = "done";
                                // 先给服务器发送 接收文件完成的消息
                                // rSocket -> write(buf);
                                rfile.close();
                                ui -> message -> append("文件接收已完成!");
                                // QMessageBox::information(this, "完成", "文件接收完成!");
                                rSocket -> disconnectFromHost();
                                rSocket -> close();
                                ui -> progressBar -> hide();
                            }
                        }

                    });
}

// 切换用户
void SmilingHome::changeUser()
{
    emit changeUsersignal(0, "change");
}

SmilingHome::~SmilingHome()
{
    Friends -> takeChildren();
    stranger -> takeChildren();
    delete Friends;
    delete stranger;
    delete ui;
}

void SmilingHome::setWindow()
{
    setWindowTitle("Smiling Home");
    // 设置窗体无边框
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    // 设置窗体背景透明
    setAttribute(Qt::WA_TranslucentBackground);
    // 设置圆角窗口
    QBitmap bmp(this->size());
    bmp.fill();
    QPainter p(&bmp);
    p.setPen(Qt::NoPen);
    p.setBrush(Qt::black);
    p.drawRoundedRect(bmp.rect(),5,5);
    setMask(bmp);

    //resize(pi -> size());
    // 获取界面的宽度
    int wide = width();
    // 构建最小化、关闭按钮
    QToolButton *minButton = new QToolButton(this);
    QToolButton *closeButton= new QToolButton(this);
    // 获取最小化、关闭按钮图标
    QPixmap minPix  = style()->standardPixmap(QStyle::SP_TitleBarMinButton);
    QPixmap closePix = style()->standardPixmap(QStyle::SP_TitleBarCloseButton);
    // 设置最小化、关闭按钮
    minButton -> setIcon(minPix);
    closeButton -> setIcon(closePix);
    // 将最小化、关闭按钮放在界面的位置
    minButton->setGeometry(wide-65,7,20,20);
    closeButton->setGeometry(wide-35,10,20,20);
    // 设置鼠标移至按钮的提示信息
    minButton->setToolTip(tr("最小化"));
    closeButton->setToolTip(tr("关闭"));
    // 设置最小化、关闭按钮风格
    minButton->setStyleSheet("background-color:transparent;");
    closeButton->setStyleSheet("background-color:transparent;");

    connect(closeButton, &QPushButton::pressed, this, &SmilingHome::windowclosed);
    connect(minButton, &QPushButton::pressed, this, &SmilingHome::windowmin);

    ui -> background -> setPixmap(QPixmap("://back.jpg"));
    ui -> background -> setScaledContents(true);
    ui -> leftLabel -> setPixmap(QPixmap("://label.jpg"));
    ui -> leftLabel -> setScaledContents(true);
    ui -> label -> setStyleSheet("color:red");
    // 自定义用户按钮
    QString btnStyle = "\
            QPushButton{\
                color: rgb(255, 0, 0);\
                border:1px;\
            }\
            QPushButton:hover{\
                color: rgb(38, 133, 227);\
            }\
            QPushButton:pressed{\
                color: rgb(255, 0, 0);\
            }";
    ui -> changeuser -> setStyleSheet(btnStyle);
    ui -> logoff -> setStyleSheet(btnStyle);
    ui -> refresh -> setStyleSheet(btnStyle);
    // 自定义删除按钮
    QString btnStyle2 =
            "QPushButton{\
                color: rgb(255, 0, 0);\
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgb(255,255,255), stop:0.3 rgb(255,255,255), stop:1 rgb(255,255,255));\
                border:1px;\
                border-radius:5px; /*border-radius控制圆角大小*/\
                padding:2px 4px;  \
            }\
            QPushButton:hover{\
                color: rgb(255, 255, 0); \
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgb(4, 186, 251), stop:0.3 rgb(4, 186, 251), stop:1 rgb(4, 186, 251));\
                border:1px;  \
                border-radius:5px; /*border-radius控制圆角大小*/\
                padding:2px 4px; \
            }\
            QPushButton:pressed{    \
                color: rgb(255, 255, 255); \
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgb(0, 0, 0), stop:0.3 rgb(0, 0, 0), stop:1 rgb(0, 0, 0));  \
                border:1px;  \
                border-radius:5px; /*border-radius控制圆角大小*/\
                padding:2px 4px; \
            }";
    ui -> delfriend -> setStyleSheet(btnStyle2);
    QString btnStyle3 =
            "QPushButton{\
                color: rgb(255, 255, 0);\
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgb(0,0,0), stop:0.3 rgb(0,0,0), stop:1 rgb(0,0,0)); \
                border:10px;\
                border-radius:0px; /*border-radius控制圆角大小*/\
                padding:10px 10px;  \
            }\
            QPushButton:hover{\
                color: rgb(255, 255, 0);\
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgb(0,0,0), stop:0.3 rgb(0,0,0), stop:1 rgb(0,0,0)); \
                border:10px;  \
                border-radius:0px; /*border-radius控制圆角大小*/\
                padding:10px 10px; \
            }\
            QPushButton:pressed{\
                color: rgb(4,186,251);\
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgb(0,0,0), stop:0.3 rgb(0,0,0), stop:1 rgb(0,0,0)); \
                border:10px;  \
                border-radius:0px; /*border-radius控制圆角大小*/\
                padding:10px 10px; \
            }";
    ui -> mainButton -> setStyleSheet(btnStyle3);
    ui -> manyButton -> setStyleSheet(btnStyle3);
    ui -> timeButton -> setStyleSheet(btnStyle3);
    ui -> moreButton -> setStyleSheet(btnStyle3);
    ui -> noline -> setStyleSheet(btnStyle);

    int rand = qrand() % 10;
    switch(rand)
    {
    case 0:
        ui -> head -> setPixmap(QPixmap("://h0.jpg"));
        ui -> head -> setScaledContents(true);
        break;
    case 1:
        ui -> head -> setPixmap(QPixmap("://h1.jpg"));
        ui -> head -> setScaledContents(true);
        break;
    case 2:
        ui -> head -> setPixmap(QPixmap("://h2.jpg"));
        ui -> head -> setScaledContents(true);
        break;
    case 3:
        ui -> head -> setPixmap(QPixmap("://h3.jpg"));
        ui -> head -> setScaledContents(true);
        break;
    case 4:
        ui -> head -> setPixmap(QPixmap("://h4.jpg"));
        ui -> head -> setScaledContents(true);
        break;
    case 5:
        ui -> head -> setPixmap(QPixmap("://h5.jpg"));
        ui -> head -> setScaledContents(true);
        break;
    case 6:
        ui -> head -> setPixmap(QPixmap("://h6.jpg"));
        ui -> head -> setScaledContents(true);
        break;
    case 7:
        ui -> head -> setPixmap(QPixmap("://h7.jpg"));
        ui -> head -> setScaledContents(true);
        break;
    case 8:
        ui -> head -> setPixmap(QPixmap("://h8.jpg"));
        ui -> head -> setScaledContents(true);
        break;
    }
}

void SmilingHome::windowclosed()
{
    if (QMessageBox::Yes == QMessageBox::question(this, "bye?", "Bye Bye?"))
    {
        this -> close();
    }
}

void SmilingHome::windowmin()
{
    this->showMinimized();
}

void SmilingHome::mousePressEvent(QMouseEvent* mEvent)
{
    if(mEvent->button()==Qt::LeftButton)
    {
        dragPosition = mEvent->globalPos()-frameGeometry().topLeft();
        qDebug()<<mEvent->globalPos();
        mEvent->accept();
    }
}
void SmilingHome::mouseMoveEvent(QMouseEvent* mEvent)
{
    if(mEvent->buttons()& Qt::LeftButton)
    {
        move(mEvent->globalPos() - dragPosition);
        qDebug()<<mEvent->globalPos();
        mEvent->accept();
    }
}

void SmilingHome::setTime()
{
    QDateTime time = QDateTime::currentDateTime();
    QString str = time.toString("yyyy-MM-dd hh:mm:ss dddd");
    ui-> time ->setText(str);
}

void SmilingHome::listFriends()
{
    /*QTreeWidgetItem *bb = new QTreeWidgetItem(Friends);
    bb -> setText(0,"xiaozhang");
    bb -> setIcon(0,QIcon(tr("://h8.jpg")));

    QTreeWidgetItem *mm = new QTreeWidgetItem(Friends);
    mm -> setText(0,"xiaowang");
    mm -> setIcon(0,QIcon("://h5.jpg"));

    QTreeWidgetItem *sis = new QTreeWidgetItem(Friends);
    sis -> setText(0,"xiaoli");
    sis -> setIcon(0,QIcon("://h4.jpg"));

    QTreeWidgetItem *xiaoming=new QTreeWidgetItem(QStringList(QString("xiaoming")));
    xiaoming->setIcon(0, QIcon(tr("://h6.jpg")));
    Friends -> addChild(xiaoming);*/

    QTreeWidgetItem *Bruce = new QTreeWidgetItem(stranger);
    Bruce -> setText(0,"Bruce");
    Bruce -> setIcon(0,QIcon(tr("://h0.jpg")));
    QTreeWidgetItem *xiaowang = new QTreeWidgetItem(stranger);
    xiaowang -> setText(0,"xiaowang");
    xiaowang -> setIcon(0,QIcon("://h1.jpg"));
    QTreeWidgetItem *dali = new QTreeWidgetItem(stranger);
    dali -> setText(0,"Xavi");
    dali -> setIcon(0,QIcon("://h2.jpg"));
    QTreeWidgetItem *Navigator = new QTreeWidgetItem(stranger);
    Navigator -> setText(0,"Navigator");
    Navigator -> setIcon(0,QIcon(tr("://h3.jpg")));
    QTreeWidgetItem *oubc=new QTreeWidgetItem(QStringList(QString("oubc")));
    oubc -> setIcon(0, QIcon(tr("://h4.jpg")));
    stranger -> addChild(oubc);
    QTreeWidgetItem *James=new QTreeWidgetItem(QStringList(QString("James")));
    James -> setIcon(0, QIcon(tr("://h5.jpg")));
    stranger -> addChild(James);
    QTreeWidgetItem *Jake=new QTreeWidgetItem(QStringList(QString("Jake")));
    Jake -> setIcon(0, QIcon(tr("://h6.jpg")));
    stranger -> addChild(Jake);
    QTreeWidgetItem *Messi=new QTreeWidgetItem(QStringList(QString("Messi")));
    Messi -> setIcon(0, QIcon(tr("://h7.jpg")));
    stranger -> addChild(Messi);
    QTreeWidgetItem *DeFong=new QTreeWidgetItem(QStringList(QString("DeFong")));
    DeFong -> setIcon(0, QIcon(tr("://h8.jpg")));
    stranger -> addChild(DeFong);
}
void SmilingHome::listDClicked(QTreeWidgetItem *item,int column)
{
    ui -> userxxx -> hide();
    ui -> headxxx -> hide();
    ui -> textEdit -> hide();
    ui -> label_5 -> hide();
    ui -> delfriend -> hide();

    strr = item -> text(column);
    headpic = item -> icon(column);
    ui -> message -> show();
    ui -> sendmessage -> show();
    ui -> tiktok -> show();
    ui -> sendfile -> show();
    ui -> send -> show();
    ui -> cancel -> show();
    ui -> xxx -> show();
    ui -> headi -> show();
    ui -> online -> show();
    ui -> recvfile -> show();

    ui -> message -> clear();
    ui -> sendmessage -> clear();
    ui -> xxx -> setText(strr);
    ui -> headi -> setIcon(headpic);

    // 网络部分
        tSocket = nullptr;
        tSocket = new QTcpSocket(this);
        // 主动和服务器建立连接
        tSocket -> connectToHost(QHostAddress(QString("127.0.0.1")), 8888);
        QString sendData = "4|" + strr;
        tSocket -> write(sendData.toLatin1());
        connect(tSocket, &QTcpSocket::readyRead,
                            [=]()
                               {
                                   // 获取对方发送的内容
                                   // 从通信套接字中取出内容
                                   QByteArray error = "error";
                                   QByteArray success = "success";
                                   QByteArray list = tSocket -> readAll();
                                   if(list == success)
                                   {
                                       ui -> online -> setText("[在线]");
                                       tSocket -> disconnectFromHost();
                                       tSocket -> close();
                                   }
                                   else if(list == error)
                                   {
                                       ui -> online -> setText("[离线]");
                                       tSocket -> disconnectFromHost();
                                       tSocket -> close();

                                      }
                               });
        QMap<QString,QString>::Iterator it = unreadmess.begin();
        while(it != unreadmess.end())
        {
            if(it.key() == strr)
            {
                ui -> message -> append(it.key() + ": " + it.value());
                if(it.value() == "抖一抖")
                {
                    TikTok();
                }
                unreadmess.erase(it);
            }
            it++;
        }
}
void SmilingHome::listClicked(QTreeWidgetItem *item, int column)
{
    strr = item -> text(column);
    headpic = item -> icon(column);
    ui -> send -> hide();
    ui -> sendfile -> hide();
    ui -> cancel -> hide();
    ui -> tiktok -> hide();
    ui -> message -> hide();
    ui -> sendmessage -> hide();
    ui -> xxx -> hide();
    ui -> headi -> hide();
    ui -> tip -> hide();

    ui -> userxxx -> show();
    ui -> headxxx -> show();
    ui -> textEdit -> show();
    ui -> label_5 -> show();
    ui -> headxxx -> setIcon(headpic);
    ui -> userxxx -> setText(strr);

    QString str0 = "Silly like a happy memory of the camera, quietly breathing air to have you around. 傻傻的像是记忆里幸福的相机，静静的呼吸身边有你的空气。";
    QString str1 = "Everyone gets tired.No one can take the pain for you. You have to go through it and grow up. ——每个人都会累，没人能为你承担所有的伤悲，人总有那么一段时间要学会自己长大。";
    QString str2 = "小时侯，幸福是一件东西，拥有就幸福；长大后，幸福是一个目标，达到就幸福；成熟后，发现幸福原来是一种心态，领悟就幸福。";
    QString str3 = "逃避不一定躲得过，面对不一定最难受；孤单不一定不快乐，得到不一定能长久；失去不一定不再有，转身不一定最软弱；别急着说别无选择，以为世上只有对于错；许多事情的答案都不是只有一个，所以我们永远有路可以走。";
    QString str4 = "很多时候，宁愿被误会，也不想去解释。信与不信，就在你一念之间。懂我的人，何必解释。";
    QString str5 = "即使你是友善的，有的人可能还会说你自私和动机不良，不管怎样，你还是要友善。你多年来营造的东西，有人在一夜之间把它摧毁，不管怎样，你还是要去营造。即使把你最好的东西给了这个世界，也许这些东西永远都不够，不管怎样，还得把你最好的东西给这个世界。";
    QString str6 = "有点怀疑自己，是不是不敢爱了。难道我也是那种一朝被蛇咬十年怕井绳的人吗?当初走的干脆，给自己留下了骄傲，不后悔。有些人是不够格去让自己多付出感情的，因为他不配。";
    QString str7 = "懒惰是很奇怪的东西，它使你以为那是安逸，是休息，是福气;但实际上它所给你的是无聊，是倦怠，是消沉;它剥夺你对前途的希望，割断你和别人之间的友情，使你心胸日渐狭窄，对人生也越来越怀疑。";
    QString str8 = "灯红酒绿闪了你的双眼，你在骚动、你却胆怯，你在向往、你却假装正经，倒不如让你来一次彻底的歇斯底里，只让我心痛一次就好。";
    QString str9 = "有时候不是不懂，只是不想懂;有时候不是不知道，只是不想说出来;有时候不是不明白，而是明白了也不知道该怎么做，于是就保持了沉默。";
    int rand = qrand() % 10;
    switch(rand)
    {
    case 0:
        ui -> textEdit -> setText(str0);
        break;
    case 1:
        ui -> textEdit -> setText(str1);
        break;
    case 2:
        ui -> textEdit -> setText(str2);
        break;
    case 3:
        ui -> textEdit -> setText(str3);
        break;
    case 4:
        ui -> textEdit -> setText(str4);
        break;
    case 5:
        ui -> textEdit -> setText(str5);
        break;
    case 6:
        ui -> textEdit -> setText(str6);
        break;
    case 7:
        ui -> textEdit -> setText(str7);
        break;
    case 8:
        ui -> textEdit -> setText(str8);
        break;
    case 9:
        ui -> textEdit -> setText(str9);
        break;
    }
}

void SmilingHome::on_refresh_clicked()
{
    ui -> progressBar -> setValue(0);

    QFile file(".//log.txt");
    bool isOk = file.open(QIODevice::ReadOnly);
    if (true == isOk)
    {
        // 往数据流中读数据，相当于在文件里读数据
        QDataStream stream(&file);
        // 读的时候，按写的顺序取数据
        stream >> username;
        //ui -> user -> setText(username);
    }
    file.close();

    ui -> ti -> clear();
    ui -> ti -> setText("请检查网络是否连接成功!");
    tSocket = nullptr;
    tSocket = new QTcpSocket(this);
    // 主动和服务器建立连接
    tSocket -> connectToHost(QHostAddress(QString("127.0.0.1")), 8888);
    connect(tSocket, &QTcpSocket::connected,
               [=]()
                   {

                   });
    QString sendData = "9|" + username;
    tSocket -> write(sendData.toLatin1());
    connect(tSocket, &QTcpSocket::readyRead,
                        [=]()
                           {
                               // 获取对方发送的内容
                               // 从通信套接字中取出内容
                               QByteArray error = "error";
                               QByteArray success = "success";
                               QByteArray list = tSocket -> readAll();
                               if(list == error)
                               {
                                   ui -> ti -> setText("载入失败...");
                                   // 主动和对方断开链接
                                   tSocket -> disconnectFromHost();
                                   tSocket -> close();
                               }
                               else
                               {
                                   QString srymd = list;
                                   QStringList ymd = srymd.split("|");
                                   birye = ymd.at(0).toInt();
                                   birmo = ymd.at(1).toInt();
                                   birda = ymd.at(2).toInt();
                                   mport = ymd.at(3).toInt() + 8000;
                                   ui -> ti -> clear();
                                   // 主动和对方断开链接
                                   tSocket -> disconnectFromHost();
                                   tSocket -> close();

                                   QPalette per;
                                   per.setColor(QPalette::WindowText, Qt::red);
                                   ui -> gs -> setAlignment(Qt::AlignRight | Qt::AlignVCenter);
                                   ui -> gm -> setAlignment(Qt::AlignRight | Qt::AlignVCenter);
                                   ui -> gD -> setAlignment(Qt::AlignRight | Qt::AlignVCenter);
                                   ui -> gM -> setAlignment(Qt::AlignRight | Qt::AlignVCenter);
                                   ui -> gY -> setAlignment(Qt::AlignRight | Qt::AlignVCenter);
                                   ui -> gs -> setPalette(per);
                                   ui -> gm -> setPalette(per);
                                   ui -> gD -> setPalette(per);
                                   ui -> gM -> setPalette(per);
                                   ui -> gY -> setPalette(per);
                                   QPalette peg;
                                   peg.setColor(QPalette::WindowText, Qt::green);
                                   QPalette peb;
                                   peb.setColor(QPalette::WindowText, Qt::blue);
                                   QPalette pey;
                                   pey.setColor(QPalette::WindowText, Qt::yellow);
                                   ui -> ws -> setPalette(per);
                                   ui -> wm -> setPalette(peg);
                                   ui -> wD -> setPalette(peb);
                                   ui -> wM -> setPalette(pey);
                                   ui -> wY -> setPalette(per);

                                   QDateTime dateTime(QDateTime::currentDateTime());
                                   QString datex = dateTime.toString("yyyy|MM|dd|hh|mm|ss|");
                                   cout << datex;
                                   QStringList datel = datex.split("|");
                                   int nowye = datel.at(0).toInt();
                                   int nowmo = datel.at(1).toInt();
                                   int nowda = datel.at(2).toInt();
                                   int nowmi = datel.at(4).toInt();
                                   int nowse = datel.at(5).toInt();


                                   cout << nowye << " " << nowmo << " " << nowda << " " << nowmi << " " << nowse;
                                   ye = nowye - birye;
                                   mo = nowmo + (ye - 1)*12 + (12 - birmo);
                                   da = mo / 2 * 31 + mo / 2 *30;
                                   mi = da * 24 *60 + nowmi;
                                   se = mi * 60 + nowse;
                                   cout << ye << " " << mo << " " << da << " " << mi << " " << se;
                                   setTimers();
                                   ui -> gY -> setText(QString::number(ye));
                                   ui -> wY -> setText(QString::number(75 - ye));
                                   ui -> gM -> setText(QString::number(mo));
                                   ui -> wM -> setText(QString::number(900 - mo));
                                   ui -> gD -> setText(QString::number(da));
                                   ui -> wD -> setText(QString::number(27350 - da));
                                   ui -> gm -> setText(QString::number(mi));
                                   ui -> wm -> setText(QString::number(39384000 - mi));

                                   // IM部分
                                   uSocket = new QUdpSocket(this);
                                   uSocket -> bind(mport);
                                   //ui -> message -> append(QString::number(8000 + mport));
                                   connect(uSocket, &QUdpSocket::readyRead,
                                               [=]()
                                                   {
                                                       char buf[1024] = {0};
                                                       QHostAddress addr;    // 对方地址
                                                       quint16 port;    // 对方端口
                                                       qint64 len = uSocket -> readDatagram(buf, sizeof(buf), &addr, &port);
                                                       if (len > 0)
                                                       {
                                                           // 格式化
                                                           QString str = QString("%1").arg(buf);
                                                           QStringList strl = str.split("|");
                                                           //编辑器设置内容
                                                           if (strr == strl.at(0))
                                                           {
                                                               if(strl.at(1) == "抖一抖")
                                                               {
                                                                   TikTok();
                                                               }
                                                               ui -> message -> append(strl.at(0) + ":" + strl.at(1));
                                                           }
                                                           else
                                                           {
                                                               unreadmess.insert(strl.at(0), strl.at(1));
                                                           }
                                                       }
                                                   });
                               }
                           });
    ui -> refresh -> setDisabled(true);
    ui -> refresh -> hide();
}

// 添加好友
void SmilingHome::on_addFriends_clicked()
{
    // 创建文件对象
    QFile file(".//add.txt");
    // 打开文件
    bool isOk = file.open(QIODevice::WriteOnly);
    if (true == isOk)
    {
        // 创建数据流，和file文件关联
        // 往数据流中输入数据，相当于往文件里写数据
        QDataStream stream(&file);
        stream << username;
    }
    file.close();

    add = new addFriend();
    add -> show();
}

// 删除好友
void SmilingHome::on_delfriend_clicked()
{
    // 网络部分
    tSocket = nullptr;
    tSocket = new QTcpSocket(this);
    // 主动和服务器建立连接
    tSocket -> connectToHost(QHostAddress(QString("127.0.0.1")), 8888);
    connect(tSocket, &QTcpSocket::connected,
           [=]()
               {
                   qDebug()<< "Success";
               });

    QString sendData = "11|" + username + "|" + strr;
    tSocket -> write(sendData.toLatin1());
    QByteArray error = "error";
    QByteArray success = "success";
    connect(tSocket, &QTcpSocket::readyRead,
                    [=]()
                       {
                           // 获取对方发送的内容
                           QByteArray tip = tSocket -> readAll();
                           if(tip == error)
                           {
                               QMessageBox::information(this, "delete", "删除失败，请稍后重试!");
                               // 主动和对方断开链接
                               tSocket -> disconnectFromHost();
                               tSocket -> close();
                           }
                           else if(tip == success)
                           {
                               QMessageBox::information(this, "delete", "删除成功，请刷新好友列表!");
                               // 主动和对方断开链接
                               tSocket -> disconnectFromHost();
                               tSocket -> close();
                           }
                       });
}

// 好友列表
void SmilingHome::on_mainButton_clicked()
{
        ui -> tip -> show();
        ui -> delfriend -> hide();
        ui -> userxxx -> hide();
        ui -> headxxx -> hide();
        ui -> textEdit -> hide();
        ui -> label_5 -> hide();
        ui -> send -> hide();
        ui -> sendfile -> hide();
        ui -> cancel -> hide();
        ui -> tiktok -> hide();
        ui -> message -> hide();
        ui -> sendmessage -> hide();
        ui -> xxx -> hide();
        ui -> headi -> hide();
        ui -> online -> hide();
        ui -> recvfile -> hide();

        ui -> page -> setCurrentIndex(1);
        ui -> list -> setColumnCount(2);
        ui -> list -> setHeaderHidden(true);
        // 网络部分
        tSocket = nullptr;
        tSocket = new QTcpSocket(this);
        // 主动和服务器建立连接
        tSocket -> connectToHost(QHostAddress(QString("127.0.0.1")), 8888);
        connect(tSocket, &QTcpSocket::connected,
                   [=]()
                       {
                           ui -> tip -> setText("     正在载入好友... ");
                       });
        QString sendData = "3|" + username;
        tSocket -> write(sendData.toLatin1());
        connect(tSocket, &QTcpSocket::readyRead,
                            [=]()
                               {
                                   // 获取对方发送的内容
                                   // 从通信套接字中取出内容
                                   QByteArray error = "error";
                                   QByteArray list = tSocket -> readAll();
                                   if(list == error)
                                   {
                                       ui -> tip -> setText("请检查网络是否连接成功!");
                                       // 主动和对方断开链接
                                       tSocket -> disconnectFromHost();
                                       tSocket -> close();
                                   }
                                   else
                                   {
                                       sr = list;
                                       srlist = sr.split('|');
                                       // 主动和对方断开链接
                                       num = srlist.at(0).toInt();
                                       tSocket -> disconnectFromHost();
                                       tSocket -> close();

                                       item = new QTreeWidgetItem[num];
                                       year = new int[num];

                                       stranger -> takeChildren();
                                       Friends -> takeChildren();

                                       for (int i = 0; i < num; i++)
                                       {
                                           item[i].setText(0, QString("%1").arg(srlist.at((2*i)+1)));
                                           int rand = qrand() % 10;
                                           switch(rand)
                                           {
                                           case 0:
                                               item[i].setIcon(0, QIcon(tr("://h0.jpg")));
                                               break;
                                           case 1:
                                               item[i].setIcon(0, QIcon(tr("://h1.jpg")));
                                               break;
                                           case 2:
                                               item[i].setIcon(0, QIcon(tr("://h2.jpg")));
                                               break;
                                           case 3:
                                               item[i].setIcon(0, QIcon(tr("://h3.jpg")));
                                               break;
                                           case 4:
                                               item[i].setIcon(0, QIcon(tr("://h4.jpg")));
                                               break;
                                           case 5:
                                               item[i].setIcon(0, QIcon(tr("://h5.jpg")));
                                               break;
                                           case 6:
                                               item[i].setIcon(0, QIcon(tr("://h6.jpg")));
                                               break;
                                           case 7:
                                               item[i].setIcon(0, QIcon(tr("://h7.jpg")));
                                               break;
                                           case 8:
                                               item[i].setIcon(0, QIcon(tr("://h8.jpg")));
                                               break;
                                           }
                                           Friends -> addChild(&item[i]);
                                           year[i] = srlist.at((2*i) + 2).toInt();
                                       }
                                       listFriends();
                                       ui -> list -> addTopLevelItem(Friends);
                                       ui -> list -> addTopLevelItem(stranger);
                                       ui -> list -> expandAll();
                                       ui -> tip -> setText("好友载入完成，快去找他们一起聊天吧！");
                                   }
                               });
}

void SmilingHome::on_timeButton_clicked()
{
}

void SmilingHome::on_headi_clicked()
{
        ui -> send -> hide();
        ui -> sendfile -> hide();
        ui -> cancel -> hide();
        ui -> tiktok -> hide();
        ui -> message -> hide();
        ui -> sendmessage -> hide();
        ui -> xxx -> hide();
        ui -> headi -> hide();
        ui -> tip -> hide();
        ui -> online -> hide();
        ui -> recvfile -> hide();

        ui -> userxxx -> show();
        ui -> headxxx -> show();
        ui -> textEdit -> show();
        ui -> label_5 -> show();
        ui -> delfriend -> show();
        ui -> headxxx -> setIcon(headpic);
        ui -> userxxx -> setText(strr);

        QString str0 = "Silly like a happy memory of the camera, quietly breathing air to have you around. 傻傻的像是记忆里幸福的相机，静静的呼吸身边有你的空气。";
        QString str1 = "Everyone gets tired.No one can take the pain for you. You have to go through it and grow up. ——每个人都会累，没人能为你承担所有的伤悲，人总有那么一段时间要学会自己长大。";
        QString str2 = "小时侯，幸福是一件东西，拥有就幸福；长大后，幸福是一个目标，达到就幸福；成熟后，发现幸福原来是一种心态，领悟就幸福。";
        QString str3 = "逃避不一定躲得过，面对不一定最难受；孤单不一定不快乐，得到不一定能长久；失去不一定不再有，转身不一定最软弱；别急着说别无选择，以为世上只有对于错；许多事情的答案都不是只有一个，所以我们永远有路可以走。";
        QString str4 = "很多时候，宁愿被误会，也不想去解释。信与不信，就在你一念之间。懂我的人，何必解释。";
        QString str5 = "即使你是友善的，有的人可能还会说你自私和动机不良，不管怎样，你还是要友善。你多年来营造的东西，有人在一夜之间把它摧毁，不管怎样，你还是要去营造。即使把你最好的东西给了这个世界，也许这些东西永远都不够，不管怎样，还得把你最好的东西给这个世界。";
        QString str6 = "有点怀疑自己，是不是不敢爱了。难道我也是那种一朝被蛇咬十年怕井绳的人吗?当初走的干脆，给自己留下了骄傲，不后悔。有些人是不够格去让自己多付出感情的，因为他不配。";
        QString str7 = "懒惰是很奇怪的东西，它使你以为那是安逸，是休息，是福气;但实际上它所给你的是无聊，是倦怠，是消沉;它剥夺你对前途的希望，割断你和别人之间的友情，使你心胸日渐狭窄，对人生也越来越怀疑。";
        QString str8 = "灯红酒绿闪了你的双眼，你在骚动、你却胆怯，你在向往、你却假装正经，倒不如让你来一次彻底的歇斯底里，只让我心痛一次就好。";
        QString str9 = "有时候不是不懂，只是不想懂;有时候不是不知道，只是不想说出来;有时候不是不明白，而是明白了也不知道该怎么做，于是就保持了沉默。";
        int rand = qrand() % 10;
        switch(rand)
        {
        case 0:
            ui -> textEdit -> setText(str0);
            break;
        case 1:
            ui -> textEdit -> setText(str1);
            break;
        case 2:
            ui -> textEdit -> setText(str2);
            break;
        case 3:
            ui -> textEdit -> setText(str3);
            break;
        case 4:
            ui -> textEdit -> setText(str4);
            break;
        case 5:
            ui -> textEdit -> setText(str5);
            break;
        case 6:
            ui -> textEdit -> setText(str6);
            break;
        case 7:
            ui -> textEdit -> setText(str7);
            break;
        case 8:
            ui -> textEdit -> setText(str8);
            break;
        case 9:
            ui -> textEdit -> setText(str9);
            break;
        }
}

void SmilingHome::TikTok()
{
    QPropertyAnimation *animation = new QPropertyAnimation(this,"geometry");
    animation->setDuration(500);
    animation->setKeyValueAt(0,QRect(QPoint(this->frameGeometry().x()-3,this->frameGeometry().y()),this->size()));
    animation->setKeyValueAt(0.1,QRect(QPoint(this->frameGeometry().x()+6,this->frameGeometry().y()),this->size()));
    animation->setKeyValueAt(0.2,QRect(QPoint(this->frameGeometry().x()-6,this->frameGeometry().y()),this->size()));
    animation->setKeyValueAt(0.3,QRect(QPoint(this->frameGeometry().x()+6,this->frameGeometry().y()),this->size()));
    animation->setKeyValueAt(0.4,QRect(QPoint(this->frameGeometry().x()-6,this->frameGeometry().y()),this->size()));
    animation->setKeyValueAt(0.5,QRect(QPoint(this->frameGeometry().x()+6,this->frameGeometry().y()),this->size()));
    animation->setKeyValueAt(0.6,QRect(QPoint(this->frameGeometry().x()-6,this->frameGeometry().y()),this->size()));
    animation->setKeyValueAt(0.7,QRect(QPoint(this->frameGeometry().x()+6,this->frameGeometry().y()),this->size()));
    animation->setKeyValueAt(0.8,QRect(QPoint(this->frameGeometry().x()-6,this->frameGeometry().y()),this->size()));
    animation->setKeyValueAt(0.9,QRect(QPoint(this->frameGeometry().x()+6,this->frameGeometry().y()),this->size()));
    animation->setKeyValueAt(1,QRect(QPoint(this->frameGeometry().x()-3,this->frameGeometry().y()),this->size()));
    animation->start();
}

void SmilingHome::on_noline_clicked()
{
    // 网络部分
    tSocket = nullptr;
    tSocket = new QTcpSocket(this);
    // 主动和服务器建立连接
    tSocket -> connectToHost(QHostAddress(QString("127.0.0.1")), 8888);
    QString sendData = "5|" + username;
    tSocket -> write(sendData.toLatin1());
}

void SmilingHome::on_addpic_clicked()
{
    QString path = QFileDialog::getOpenFileName(this,"open", "../","TXT(*.jpg *.png *.jpeg)");
    if (path.isEmpty() == false)
    {}
    else
    {
        cout << "文件路径为空!";
    }

    QTextDocumentFragment fragment;
    ui -> note -> append("\n");
    QString pa = QString("<img src='%1' width='400'>").arg(path);
    fragment = QTextDocumentFragment::fromHtml(pa);
    ui -> note -> textCursor().insertFragment(fragment);
    ui -> note -> setVisible(true);
    ui -> note -> append("\n");
}

void SmilingHome::on_addtime_clicked()
{
    QDateTime time = QDateTime::currentDateTime();
    QString str = time.toString("yyyy-MM-dd hh:mm:ss dddd");
    ui -> note -> append("时间："+ str + "\n");
}

void SmilingHome::on_addevent_clicked()
{
    ui -> note -> append("事件：\n");
}

void SmilingHome::on_pushButton_clicked()
{
        QString Path = QFileDialog::getExistingDirectory(
                       this, "choose src Directory",
                        "/");

            if (Path.isEmpty())
            {
                return;
            }
            else
            {
                qDebug() << "srcDirPath=" << Path;
                Path += "/";
            }
        QString filename = QString("%1/SmileNote.doc").arg(Path);
        QFile file(filename);
        file.open(QFile::WriteOnly);
        QTextStream writefiletext(&file);
        writefiletext << (ui -> note -> toPlainText()).toUtf8();
        file.close();
}

void SmilingHome::on_calendar_clicked(const QDate &date)
{
        ui -> event -> clear();
        ui -> tips -> clear();
        selectDate = date.toString("yyyyMMdd");
        tSocket = nullptr;
        tSocket = new QTcpSocket(this);
        // 主动和服务器建立连接
        tSocket -> connectToHost(QHostAddress(QString("127.0.0.1")), 8888);
        connect(tSocket, &QTcpSocket::connected,
                   [=]()
                       {
                           ui -> tips -> setText("正在载入事务... ");
                       });
        QString sendData = "6|" + username + "|" + selectDate;
        tSocket -> write(sendData.toLatin1());
        connect(tSocket, &QTcpSocket::readyRead,
                            [=]()
                               {
                                   // 获取对方发送的内容
                                   // 从通信套接字中取出内容
                                   QByteArray error = "error";
                                   QByteArray success = "success";
                                   QByteArray list = tSocket -> readAll();
                                   if(list == error)
                                   {
                                       ui -> tips -> setText("载入失败！");
                                       // 主动和对方断开链接
                                       tSocket -> disconnectFromHost();
                                       tSocket -> close();
                                   }
                                   else if(list == success)
                                   {
                                       ui -> tips -> setText("该日期没有待办事务哟!");
                                       // 主动和对方断开链接
                                       tSocket -> disconnectFromHost();
                                       tSocket -> close();
                                   }
                                   else
                                   {
                                       QString str = list;
                                       ui -> tips -> setText("载入完成！");
                                       ui -> event -> setText(str);
                                       // 主动和对方断开链接
                                       tSocket -> disconnectFromHost();
                                       tSocket -> close();
                                   }
                               });
}

void SmilingHome::on_addevents_clicked()
{
        ui -> tips -> clear();
        // 网络部分
        QString str = ui -> event -> toPlainText();
        if (str == nullptr)
        {
            ui -> tips -> setText("内容不能为空!");
        }
        else
        {
            tSocket = nullptr;
            tSocket = new QTcpSocket(this);
            // 主动和服务器建立连接
            tSocket -> connectToHost(QHostAddress(QString("127.0.0.1")), 8888);
            connect(tSocket, &QTcpSocket::connected,
                       [=]()
                           {
                               ui -> tips -> setText("正在添加事务... ");
                           });
            QString sendData = "7|" + username + "|" + selectDate + "|" + str;
            tSocket -> write(sendData.toLatin1());
            connect(tSocket, &QTcpSocket::readyRead,
                                [=]()
                                   {
                                       // 获取对方发送的内容
                                       // 从通信套接字中取出内容
                                       QByteArray error = "error";
                                       QByteArray success = "success";
                                       QByteArray list = tSocket -> readAll();
                                       if(list == error)
                                       {
                                           ui -> tips -> setText("添加事务失败，请重新尝试!");
                                           // 主动和对方断开链接
                                           tSocket -> disconnectFromHost();
                                           tSocket -> close();
                                       }
                                       if(list == success)
                                       {
                                           ui -> tips -> setText("添加事务成功!");
                                           // 主动和对方断开链接
                                           tSocket -> disconnectFromHost();
                                           tSocket -> close();
                                       }
                                   });
        }
}

void SmilingHome::on_delevent_clicked()
{
        ui -> tips -> clear();
        tSocket = nullptr;
        tSocket = new QTcpSocket(this);
        // 主动和服务器建立连接
        tSocket -> connectToHost(QHostAddress(QString("127.0.0.1")), 8888);
        connect(tSocket, &QTcpSocket::connected,
                   [=]()
                       {
                           ui -> tips -> setText("正在删除事务... ");
                       });
        QString sendData = "8|" + username + "|" + selectDate;
        tSocket -> write(sendData.toLatin1());
        connect(tSocket, &QTcpSocket::readyRead,
                            [=]()
                               {
                                   // 获取对方发送的内容
                                   // 从通信套接字中取出内容
                                   QByteArray error = "error";
                                   QByteArray success = "success";
                                   QByteArray list = tSocket -> readAll();
                                   if(list == error)
                                   {
                                       ui -> tips -> setText("删除事务失败，请重新尝试!");
                                       // 主动和对方断开链接
                                       tSocket -> disconnectFromHost();
                                       tSocket -> close();
                                   }
                                   if(list == success)
                                   {
                                       ui -> tips -> setText("删除事务成功!");
                                       ui -> event -> clear();
                                       // 主动和对方断开链接
                                       tSocket -> disconnectFromHost();
                                       tSocket -> close();
                                   }
                               });
}

void SmilingHome::on_recvfile_clicked()
{
        // 开始连接
        rSocket -> connectToHost(QHostAddress("127.0.0.1"), 6666);
        ui -> progressBar -> show();
        ui -> message -> append("已就绪，等待对方发送文件...");
}

void SmilingHome::on_tiktok_clicked()
{
        tSocket = nullptr;
        tSocket = new QTcpSocket(this);
        // 主动和服务器建立连接
        tSocket -> connectToHost(QHostAddress(QString("127.0.0.1")), 8888);
        connect(tSocket, &QTcpSocket::connected,
                   [=]()
                       {

                       });
        QString sendData = "10|" + strr;
        tSocket -> write(sendData.toLatin1());
        connect(tSocket, &QTcpSocket::readyRead,
                            [=]()
                               {
                                   // 获取对方发送的内容
                                   // 从通信套接字中取出内容
                                   QByteArray error = "error";
                                   QByteArray list = tSocket -> readAll();
                                   if(list == error)
                                   {
                                       // 主动和对方断开链接
                                       tSocket -> disconnectFromHost();
                                       tSocket -> close();
                                   }
                                   else
                                   {
                                       yport = list.toInt() + 8000;
                                       // 主动和对方断开链接
                                       tSocket -> disconnectFromHost();
                                       tSocket -> close();

                                       // 发送数据
                                       // 获取编辑区的内容
                                       QString str = username + "|" + "抖一抖" + "|";
                                       // 向指定的IP发送数据
                                       uSocket -> writeDatagram(str.toUtf8(), QHostAddress("127.0.0.1"), yport);
                                       ui -> message -> append("我: 抖一抖");
                                   }
                               });
        TikTok();
}

void SmilingHome::setGit()
{
    ui -> git -> setText("<h4><a href=\"https://github.com/oubc\">[oubc]</a></h4>>");
    // 打开外部链接
    ui -> git -> setOpenExternalLinks(true);
}

void SmilingHome::setTimers()
{
    daTimer = new QTimer(this);
    if (daTimer -> isActive() == false)
    {
        daTimer -> start(86400000);
    }
    connect(daTimer,&QTimer::timeout,
                [=]()
                    {
                        da ++;
                    });
    miTimer = new QTimer(this);
    if (miTimer -> isActive() == false)
    {
        miTimer -> start(60000);
    }
    connect(miTimer,&QTimer::timeout,
                [=]()
                    {
                        mi ++;
                    });
    seTimer = new QTimer(this);
    if (seTimer -> isActive() == false)
    {
        seTimer -> start(1000);
    }
    connect(seTimer,&QTimer::timeout,
                [=]()
                    {
                        static long long int i = se;
                        ui -> gs -> setText(QString::number(i));
                        ui -> ws -> setText(QString::number(2363040000 -i ));
                        i ++;
                    });
}

void SmilingHome::on_sendfile_clicked()
{
        ui -> progressBar -> setMinimum(0);    // 最小值
        ui -> progressBar -> setValue(0);    // 当前值

        QString filePath = QFileDialog::getOpenFileName(this, "open", "../");
        // 如果选择文件有效
        if (false == filePath.isEmpty())
        {
            sfileName.clear();
            sfileSize = 0;
            // 读文件信息
            QFileInfo info(filePath);
            sfileName = info.fileName();    // 获取文件名字
            sfileSize = info.size();    // 获取文件大小
            ui -> progressBar -> setMaximum(sfileSize/1024);    // 最大值
            sendSize = 0;
            // 只读方式打开文件
            // 指定文件名字
            sfile.setFileName(filePath);
            // 打开文件
            bool isOk = sfile.open(QIODevice::ReadOnly);
            if (false == isOk)
            {
                ui -> message -> append("文件: 只读方式打开文件失败！");
            }
            // 提示打开文件的路径
            ui -> message -> append("文件: " + filePath);

            // 先发送文件头信息
            QString head = QString("%1##%2").arg(sfileName).arg(sfileSize);
            qint64 len = fSocket -> write(head.toUtf8());
            if (len > 0)    // 头部信息发送成功
            {
                // 发送真正的文件信息
                // 防止TCP黏包文件
                // 需要通过定时器延时 20ms
                ftimer.start(2000);
            }
            else
            {
                ui -> message -> append("文件: 发生失败，请重新尝试");
                sfile.close();
            }
        }
        else
        {
            ui -> message -> append("文件: 选择文件路径无效！");
        }
}

void SmilingHome::on_cancel_clicked()
{
    ui -> sendmessage -> clear();
}

void SmilingHome::on_send_clicked()
{
        if (ui -> sendmessage -> toPlainText() == nullptr)
        {
        }
        else
        {
            tSocket = nullptr;
            tSocket = new QTcpSocket(this);
            // 主动和服务器建立连接
            tSocket -> connectToHost(QHostAddress(QString("127.0.0.1")), 8888);
            connect(tSocket, &QTcpSocket::connected,
                       [=]()
                           {

                           });
            QString sendData = "10|" + strr;
            tSocket -> write(sendData.toLatin1());
            connect(tSocket, &QTcpSocket::readyRead,
                                [=]()
                                   {
                                       // 获取对方发送的内容
                                       // 从通信套接字中取出内容
                                       QByteArray error = "error";
                                       QByteArray list = tSocket -> readAll();
                                       if(list == error)
                                       {
                                           // 主动和对方断开链接
                                           tSocket -> disconnectFromHost();
                                           tSocket -> close();
                                       }
                                       else
                                       {
                                           yport = list.toInt() + 8000;
                                           // 主动和对方断开链接
                                           tSocket -> disconnectFromHost();
                                           tSocket -> close();

                                           // 发送数据
                                           // 获取编辑区的内容
                                           QString str = username + "|" + ui -> sendmessage -> toPlainText() + "|";
                                           // 向指定的IP发送数据
                                           uSocket -> writeDatagram(str.toUtf8(), QHostAddress("127.0.0.1"), yport);
                                           ui -> message -> append("我: " + ui -> sendmessage -> toPlainText());
                                           ui -> sendmessage -> clear();
                                       }
                                   });
        }
}

void SmilingHome::sendData()
{
    qint64 len = 0;
    do
    {
        // 每次发送数据的大小为4K
        char buf[4 * 1024] = {0};
        len = 0;
        // 往文件中读数据
        len = sfile.read(buf, sizeof(buf));
        // 发送数据，读多少，发多少
        len = fSocket -> write(buf, len);
        // 发送的数据需要累加
        sendSize += len;
        // 更新进度条
        ui -> progressBar -> setValue(sendSize / 1024);
    }while(sendSize != sfileSize);

    // 是否发送文件完毕
    if (sendSize == sfileSize)
    {
        ui -> message -> append("文件发送完毕!");
        sfile.close();
        // 把客户端断开
        fSocket -> disconnectFromHost();
        fSocket -> close();
        ui -> progressBar -> hide();
    }
}

