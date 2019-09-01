#include "addfriend.h"
#include "ui_addfriend.h"

addFriend::addFriend(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::addFriend)
{
    ui->setupUi(this);
    setWindow();
}

addFriend::~addFriend()
{
    delete ui;
}

void addFriend::setWindow()
{
    setWindowTitle("Smiley face");
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
    ui -> background -> setPixmap(QPixmap("://white.jpg"));

    //resize(pi -> size());
    // 获取界面的宽度
    int wide = width();
    // 构建最小化、关闭按钮
    QToolButton *minButton = new QToolButton(this);
    QToolButton *closeButton= new QToolButton(this);
    // 获取最小化、关闭按钮图标
    QPixmap minPix  = style() -> standardPixmap(QStyle::SP_TitleBarMinButton);
    QPixmap closePix = style() -> standardPixmap(QStyle::SP_TitleBarCloseButton);
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

    connect(closeButton, &QPushButton::pressed, this, &addFriend::windowclosed);
    connect(minButton, &QPushButton::pressed, this, &addFriend::windowmin);

    // 自定义按钮
    QString btnStyle1 =
            "QPushButton{\
                color: rgb(255, 255, 0);\
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgb(4,186,251), stop:0.3 rgb(4,186,251), stop:1 rgb(4,186,251));\
                border:1px;\
                border-radius:5px; /*border-radius控制圆角大小*/\
                padding:2px 4px;  \
            }\
            QPushButton:hover{\
                color: rgb(255, 255, 255); \
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgb(4,186,251), stop:0.3 rgb(4,186,251), stop:1 rgb(4,186,251));\
                border:1px;  \
                border-radius:5px; /*border-radius控制圆角大小*/\
                padding:2px 4px; \
            }\
            QPushButton:pressed{    \
                color: rgb(255, 255, 0); \
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgb(0,0,200), stop:0.3 rgb(0,0,230), stop:1 rgb(0,0,255));  \
                border:1px;  \
                border-radius:5px; /*border-radius控制圆角大小*/\
                padding:2px 4px; \
            }";
    ui -> addButton -> setStyleSheet(btnStyle1);
}
void addFriend::windowclosed()
{
    this->close();
}
void addFriend::windowmin()
{
    this->showMinimized();
}
void addFriend::mousePressEvent(QMouseEvent* mEvent)
{
    if(mEvent -> button()==Qt::LeftButton)
    {
        dragPosition = mEvent -> globalPos()-frameGeometry().topLeft();
        qDebug() << mEvent -> globalPos();
        mEvent->accept();
    }
}
void addFriend::mouseMoveEvent(QMouseEvent* mEvent)
{
    if(mEvent -> buttons()& Qt::LeftButton)
    {
        move(mEvent -> globalPos() - dragPosition);
        qDebug()<< mEvent -> globalPos();
        mEvent -> accept();
    }
}

void addFriend::on_addButton_clicked()
{
    ui -> tip -> setText("提示:");
    QFile file(".//log.txt");
    bool isOk = file.open(QIODevice::ReadOnly);
    if (true == isOk)
    {
        // 往数据流中读数据，相当于在文件里读数据
        QDataStream stream(&file);
        // 读的时候，按写的顺序取数据
        stream >> username;
    }
    file.close();

    addUsername = ui -> lineEdit -> text();
    if (addUsername != nullptr)
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

        QString sendData = "2|" + username + "|" + addUsername;
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
                                   addFriend::TikTok();
                                   ui -> tip -> setText("提示:你输入的用户不存在！");
                                   ui -> lineEdit -> clear();
                                   addFriend::TikTok();
                                   // 主动和对方断开链接
                                   tSocket -> disconnectFromHost();
                                   tSocket -> close();
                               }
                               else if(tip == success)
                               {
                                   ui -> tip -> setText("添加成功，快去和新友沟通吧！");
                                   // 主动和对方断开链接
                                   tSocket -> disconnectFromHost();
                                   tSocket -> close();
                               }
                           });
    }
    else
    {
        ui -> tip -> setText("提示:请输入正确的用户名！");
        addFriend::TikTok();
    }
}

void addFriend::TikTok()
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
