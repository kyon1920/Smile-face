#include "registered.h"
#include "ui_registered.h"

Registered::Registered(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Registered)
{
    ui->setupUi(this);
    setWindow();
    setGit();
    // 随机数种子
    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
}

void Registered::setWindow()
{
    setWindowTitle("Registered");
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

    connect(closeButton, &QPushButton::pressed, this, &Registered::windowclosed);
    connect(minButton, &QPushButton::pressed, this, &Registered::windowmin);
    ui -> background -> setPixmap(QPixmap("://background.jpg"));
    //ui -> background -> setScaledContents(true);
    ui -> picture -> setPixmap(QPixmap("://picture.jpg"));
    connect(ui -> gologin, &QPushButton::clicked, this, &Registered::goLoginsignals);

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
    ui -> sure -> setStyleSheet(btnStyle1);
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
    ui -> gologin -> setStyleSheet(btnStyle2);
    QString btnStyle3 =
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
    ui -> exit -> setStyleSheet(btnStyle3);
    int n = qrand() % 9;    //产生9以内的随机数
    switch(n)
    {
        case 0:
            ui -> picture -> setPixmap(QPixmap("://h0.jpg"));
            ui -> picture -> setScaledContents(true);
            break;
        case 1:
            ui -> picture -> setPixmap(QPixmap("://h1.jpg"));
            ui -> picture -> setScaledContents(true);
            break;
        case 2:
            ui -> picture -> setPixmap(QPixmap("://h2.jpg"));
            ui -> picture -> setScaledContents(true);
            break;
        case 3:
            ui -> picture -> setPixmap(QPixmap("://h3.jpg"));
            ui -> picture -> setScaledContents(true);
            break;
        case 4:
            ui -> picture -> setPixmap(QPixmap("://h4.jpg"));
            ui -> picture -> setScaledContents(true);
            break;
        case 5:
            ui -> picture -> setPixmap(QPixmap("://h5.jpg"));
            ui -> picture -> setScaledContents(true);
            break;
        case 6:
            ui -> picture -> setPixmap(QPixmap("://h6.jpg"));
            ui -> picture -> setScaledContents(true);
            break;
        case 7:
            ui -> picture -> setPixmap(QPixmap("://h7.jpg"));
            ui -> picture -> setScaledContents(true);
            break;
        case 8:
            ui -> picture -> setPixmap(QPixmap("://h8.jpg"));
            ui -> picture -> setScaledContents(true);
            break;
    }
    // 设置密码模式
    ui -> password -> setEchoMode(QLineEdit::Password);
    ui -> surepassword -> setEchoMode(QLineEdit::Password);
}

void Registered::windowclosed()
{
    // 主动和对方断开链接
    tcpSocket -> disconnectFromHost();
    tcpSocket -> close();
    this->close();
}

void Registered::windowmin()
{
    this->showMinimized();
}

void Registered::mousePressEvent(QMouseEvent* mEvent)
{
    if(mEvent->button()==Qt::LeftButton)
    {
        dragPosition = mEvent->globalPos()-frameGeometry().topLeft();
        qDebug()<<mEvent->globalPos();
        mEvent->accept();
    }
}
void Registered::mouseMoveEvent(QMouseEvent* mEvent)
{
    if(mEvent->buttons()& Qt::LeftButton)
    {
        move(mEvent->globalPos() - dragPosition);
        qDebug()<<mEvent->globalPos();
        mEvent->accept();
    }
}

void Registered::goLoginsignals()
{
    emit goLoginsignal(0, "change");
}

Registered::~Registered()
{
    delete ui;
}

void Registered::on_exit_clicked()
{
    this -> close();
}

void Registered::on_select_clicked()
{
    QString path = QFileDialog::getOpenFileName(this,"open","../","TXT(*.jpg *.png *.jpeg)");
    if (path.isEmpty() == false)
    {
        ui -> picture -> setPixmap(QPixmap(path));
        ui -> picture -> setScaledContents(true);
    }
    else
    {
        cout << "文件路径为空!";
    }
}

void Registered::setGit()
{
    ui -> labelUrl -> setText("<h4><a href=\"https://github.com/oubc\">小海鸥bc</a></h4>>");
    // 打开外部链接
    ui -> labelUrl -> setOpenExternalLinks(true);
}

void Registered::on_sure_clicked()
{
    tcpSocket = nullptr;
    tcpSocket = new QTcpSocket(this);
    // 主动和服务器建立连接
    tcpSocket -> connectToHost(QHostAddress("127.0.0.1"), 8888);
    connect(tcpSocket, &QTcpSocket::connected,
                [=]()
                    {
                         cout << "成功和服务器建立连接";
                    });
    int su = 0;
    QString username = ui -> user -> text();
    if(username == nullptr)
    {
        Registered::TikTok();
        ui -> error -> setText("用户名不能为空");
        su = 1;
    }
    QString password = ui -> password -> text();
    if(password == nullptr)
    {
        Registered::TikTok();
        ui -> error -> setText("密码不能为空");
        su = 1;
    }
    QString surepassword = ui -> surepassword -> text();
    if(password != surepassword)
    {
        Registered::TikTok();
        ui -> error -> setText("两次密码输入不匹配");
        su = 1;
    }
    int year = ui -> year -> text().toInt();
    int month = ui -> month -> text().toInt();
    int day = ui -> day -> text().toInt();
    if(year <=1900 || year >= 2020 || month <= 0 || month >= 13 || day <= 0 || day >= 32)
    {
        Registered::TikTok();
        ui -> error -> setText("输入出生日期不对，请重新输入!");
        su = 1;
    }
    if( su == 0 )
    {
        ui -> error -> clear();
        QString sendData = "0|" + username + "|" + password + "|" + QString::number(year) + "|" + QString::number(month) + "|" + QString::number(day);
        //QByteArray date = sendData.toUtf8();
        tcpSocket -> write(sendData.toLatin1());
        ui -> error -> setText("正在处理，请稍候...");
        tip = nullptr;
        QByteArray error = "error";
        QByteArray success = "success";
        connect(tcpSocket, &QTcpSocket::readyRead,
                            [=]()
                               {
                                   // 获取对方发送的内容
                                   tip = tcpSocket -> readAll();
                                   if(tip == error)
                                   {
                                       ui -> error -> setText("注册失败，用户已存在或者网络连接出错!");
                                       Registered::TikTok();
                                       // 主动和对方断开链接
                                       tcpSocket -> disconnectFromHost();
                                       tcpSocket -> close();
                                   }
                                   else if(tip == success)
                                   {
                                       ui -> error -> clear();
                                       QMessageBox::about(this, "RE", "注册成功");
                                       // 主动和对方断开链接
                                       tcpSocket -> disconnectFromHost();
                                       tcpSocket -> close();
                                   }
                               });
    }
}

void Registered::TikTok()
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
