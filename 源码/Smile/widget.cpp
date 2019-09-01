#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    setWindow();
    // 注册-窗口切换回到登录界面
    void (Registered::*ReplaceSlot)(int, QString) = &Registered::goLoginsignal;
    connect(&r, ReplaceSlot, this, &Widget::dealgoLogin);
    void (SmilingHome::*ReplaceSlot1)(int, QString) = &SmilingHome::changeUsersignal;
    connect(&s, ReplaceSlot1, this, &Widget::dealchangeUser);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::setWindow()
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
    p.drawRoundedRect(bmp.rect(),20,20);
    setMask(bmp);

    QMovie *myMovie = new QMovie("://background.gif");
    // 设置动画
    ui -> background -> setMovie(myMovie);
    // 启动动画
    myMovie -> start();
    ui -> background -> setScaledContents(true);

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

    connect(closeButton, &QPushButton::pressed, this, &Widget::windowclosed);
    connect(minButton, &QPushButton::pressed, this, &Widget::windowmin);

    // 自定义注册按钮
    QString btnStyle1 = "\
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
    ui -> registered -> setStyleSheet(btnStyle1);
    // 自定义登录按钮
    QString btnStyle2 =
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
    ui -> sure -> setStyleSheet(btnStyle2);
    // 设置密码模式
    ui -> password -> setEchoMode(QLineEdit::Password);
}

void Widget::dealgoLogin(int, QString)
{
    r.close();
    this -> show();
}

void Widget::dealchangeUser()
{
    s.close();
    this -> show();
}

void Widget::windowclosed()
{
    this->close();
}

void Widget::windowmin()
{
    this->showMinimized();
}

void Widget::mousePressEvent(QMouseEvent* mEvent)
{
    if(mEvent->button()==Qt::LeftButton)
    {
        dragPosition = mEvent->globalPos()-frameGeometry().topLeft();
        qDebug()<<mEvent->globalPos();
        mEvent->accept();
    }
}
void Widget::mouseMoveEvent(QMouseEvent* mEvent)
{
    if(mEvent->buttons()& Qt::LeftButton)
    {
        move(mEvent->globalPos() - dragPosition);
        qDebug()<<mEvent->globalPos();
        mEvent->accept();
    }
}

void Widget::on_registered_clicked()
{
    this -> hide();
    r.show();
}

void Widget::on_sure_clicked()
{
    QString username = ui -> user -> text();
    QString password = ui -> password -> text();
    if (username != nullptr && password != nullptr)
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

        QString sendData = "1|" + username + "|" + password;
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
                                   Widget::TikTok();
                                   // 主动和对方断开链接
                                   tSocket -> disconnectFromHost();
                                   tSocket -> close();
                               }
                               else if(tip == success)
                               {
                                   // 创建文件对象
                                   QFile file(".//log.txt");
                                   // 打开文件
                                   bool isOk = file.open(QIODevice::WriteOnly);
                                   if (true == isOk)
                                   {
                                       // 创建数据流，和file文件关联
                                       // 往数据流中输入数据，相当于往文件里写数据
                                       QDataStream stream(&file);
                                       stream << ui -> user -> text();
                                   }
                                   file.close();
                                   // 主动和对方断开链接
                                   tSocket -> disconnectFromHost();
                                   tSocket -> close();
                                   this -> close();
                                   s.show();
                               }
                           });
    }
    else
    {
        Widget::TikTok();
    }
}

void Widget::TikTok()
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
