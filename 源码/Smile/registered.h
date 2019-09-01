#ifndef REGISTERED_H
#define REGISTERED_H

#include <QWidget>
#include <QPixmap>
#include <QMovie>
#include <QDebug>
#include <QString>
#include <QFile>
#include <QStyle>
#include <QFileDialog>
#include <QPushButton>
#include <QToolButton>
#include <QMouseEvent>
#include <QPixmap>
#include <QBitmap>
#include <QPainter>
#include <QTime>
#include <QTimer>
#include <ctime>
#include <QMessageBox>
#include <QTcpSocket>
#include <QHostAddress>
#include <QPlainTextDocumentLayout>
#include <QPlainTextEdit>
#include <QThread>
#include <QPropertyAnimation>

#define cout qDebug() << "[" << __FILE__ << ":" << __LINE__ << "]"

namespace Ui {
class Registered;
}

class Registered : public QWidget
{
    Q_OBJECT

public:
    explicit Registered(QWidget *parent = nullptr);
    ~Registered();
    void setWindow();
    void windowclosed();
    void windowmin();
    void mouseMoveEvent(QMouseEvent* mEvent);
    void mousePressEvent(QMouseEvent* mEvent);
    void goLoginsignals();
    void setGit();
    void TikTok();
signals:
    void goLoginsignal(int, QString);

private slots:
    void on_exit_clicked();
    void on_select_clicked();

    void on_sure_clicked();

private:
    Ui::Registered *ui;
    QPoint dragPosition;
    QTcpSocket *tcpSocket;
    QByteArray tip;
};

#endif // REGISTERED_H
