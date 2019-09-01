#ifndef CONTACT_H
#define CONTACT_H

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

namespace Ui {
class contact;
}

class contact : public QWidget
{
    Q_OBJECT

public:
    explicit contact(QWidget *parent = nullptr);
    ~contact();
    void setWindow();
    void windowclosed();
    void windowmin();
    void mouseMoveEvent(QMouseEvent* mEvent);
    void mousePressEvent(QMouseEvent* mEvent);
    void getUser();

private:
    Ui::contact *ui;
    QPoint dragPosition;
    QString username;
};

#endif // CONTACT_H
