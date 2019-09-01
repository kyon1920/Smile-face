#ifndef WIDGET_H
#define WIDGET_H
#include "registered.h"
#include "smilinghome.h"

#include <QWidget>
#include <QPainter>
#include <QMovie>
#include <QDebug>
#include <QToolButton>
#include <QStyle>
#include <QPushButton>
#include <QBitmap>
#include <QPen>
#include <QBrush>
#include <QTcpSocket>
#include <QHostAddress>
#include <QPropertyAnimation>
#include <QFile>

#define cout qDebug() << "[" << __FILE__ << ":" << __LINE__ << "]"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();
    // 设置窗体函数
    void setWindow();
    void windowclosed();
    void windowmin();
    void mouseMoveEvent(QMouseEvent* mEvent);
    void mousePressEvent(QMouseEvent* mEvent);
    void dealgoLogin(int, QString);
    void dealchangeUser();
    void TikTok();

private slots:
    void on_registered_clicked();
    void on_sure_clicked();

private:
    Ui::Widget *ui;
    QPoint dragPosition;
    Registered r;
    SmilingHome s;
    QTcpSocket *tSocket;
};

#endif // WIDGET_H
