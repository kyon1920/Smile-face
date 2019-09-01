#ifndef ADDFRIEND_H
#define ADDFRIEND_H

#include <QWidget>
#include <QPushButton>
#include <QToolButton>
#include <QPixmap>
#include <QBitmap>
#include <QPainter>
#include <QStyle>
#include <QPoint>
#include <QDebug>
#include <QString>
#include <QIcon>
#include <QMouseEvent>
#include <QString>
#include <QTcpSocket>
#include <QPropertyAnimation>
#include <QHostAddress>

namespace Ui {
class addFriend;
}

class addFriend : public QWidget
{
    Q_OBJECT

public:
    explicit addFriend(QWidget *parent = nullptr);
    ~addFriend();
    void setWindow();
    void windowclosed();
    void windowmin();
    void mouseMoveEvent(QMouseEvent* mEvent);
    void mousePressEvent(QMouseEvent* mEvent);
    void TikTok();

private slots:
    void on_addButton_clicked();

private:
    Ui::addFriend *ui;
    QPoint dragPosition;
    QString username;
    QTcpSocket *tSocket;
    QString addUsername;
};

#endif // ADDFRIEND_H
