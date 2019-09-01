#include "contact.h"
#include "ui_contact.h"

contact::contact(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::contact)
{
    ui->setupUi(this);
    setWindow();
    getUser();
}

contact::~contact()
{
    delete ui;
}

void contact::setWindow()
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
    ui -> background -> setPixmap(QPixmap("://backcon.jpg"));

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

    connect(closeButton, &QPushButton::pressed, this, &contact::windowclosed);
    connect(minButton, &QPushButton::pressed, this, &contact::windowmin);

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
    ui -> send -> setStyleSheet(btnStyle1);
    ui -> sendFile -> setStyleSheet(btnStyle1);
    ui -> cancel -> setStyleSheet(btnStyle1);
    ui -> tikTok -> setStyleSheet(btnStyle1);
}

void contact::windowclosed()
{
    this->close();
}
void contact::windowmin()
{
    this->showMinimized();
}

void contact::mousePressEvent(QMouseEvent* mEvent)
{
    if(mEvent -> button()==Qt::LeftButton)
    {
        dragPosition = mEvent -> globalPos()-frameGeometry().topLeft();
        qDebug() << mEvent -> globalPos();
        mEvent->accept();
    }
}
void contact::mouseMoveEvent(QMouseEvent* mEvent)
{
    if(mEvent -> buttons()& Qt::LeftButton)
    {
        move(mEvent -> globalPos() - dragPosition);
        qDebug()<< mEvent -> globalPos();
        mEvent -> accept();
    }
}

void contact::getUser()
{
    QFile file("H:/Smiley face/Smile/chat.txt");
    bool isOk = file.open(QIODevice::ReadOnly);
    if (true == isOk)
    {
        // 往数据流中读数据，相当于在文件里读数据
        QDataStream stream(&file);
        // 读的时候，按写的顺序取数据
        stream >> username;
        ui -> username -> setText(username);
    }
    file.close();
}
