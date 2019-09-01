/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.13.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QLabel *background;
    QPushButton *registered;
    QPushButton *sure;
    QLabel *label;
    QLineEdit *user;
    QLabel *winTitle;
    QLabel *label_2;
    QLineEdit *password;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QString::fromUtf8("Widget"));
        Widget->resize(400, 450);
        Widget->setMinimumSize(QSize(400, 450));
        Widget->setMaximumSize(QSize(400, 450));
        background = new QLabel(Widget);
        background->setObjectName(QString::fromUtf8("background"));
        background->setGeometry(QRect(0, 0, 400, 450));
        background->setMinimumSize(QSize(400, 450));
        background->setMaximumSize(QSize(400, 450));
        registered = new QPushButton(Widget);
        registered->setObjectName(QString::fromUtf8("registered"));
        registered->setGeometry(QRect(290, 410, 93, 28));
        QFont font;
        font.setFamily(QString::fromUtf8("\345\215\216\346\226\207\346\245\267\344\275\223"));
        font.setPointSize(11);
        registered->setFont(font);
        sure = new QPushButton(Widget);
        sure->setObjectName(QString::fromUtf8("sure"));
        sure->setGeometry(QRect(190, 360, 151, 28));
        QFont font1;
        font1.setFamily(QString::fromUtf8("\345\215\216\346\226\207\346\245\267\344\275\223"));
        font1.setPointSize(11);
        font1.setBold(false);
        font1.setUnderline(false);
        font1.setWeight(50);
        sure->setFont(font1);
        label = new QLabel(Widget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(140, 280, 65, 25));
        label->setMinimumSize(QSize(65, 25));
        label->setMaximumSize(QSize(65, 25));
        QFont font2;
        font2.setFamily(QString::fromUtf8("\345\215\216\346\226\207\350\241\214\346\245\267"));
        font2.setPointSize(11);
        label->setFont(font2);
        user = new QLineEdit(Widget);
        user->setObjectName(QString::fromUtf8("user"));
        user->setGeometry(QRect(220, 280, 150, 25));
        user->setMinimumSize(QSize(150, 25));
        user->setMaximumSize(QSize(150, 25));
        QFont font3;
        font3.setFamily(QString::fromUtf8("\345\215\216\346\226\207\350\241\214\346\245\267"));
        font3.setPointSize(11);
        font3.setBold(true);
        font3.setWeight(75);
        user->setFont(font3);
        winTitle = new QLabel(Widget);
        winTitle->setObjectName(QString::fromUtf8("winTitle"));
        winTitle->setGeometry(QRect(10, 0, 100, 25));
        winTitle->setMinimumSize(QSize(100, 25));
        winTitle->setMaximumSize(QSize(100, 25));
        QFont font4;
        font4.setFamily(QString::fromUtf8("Consolas"));
        font4.setPointSize(10);
        font4.setBold(true);
        font4.setWeight(75);
        winTitle->setFont(font4);
        label_2 = new QLabel(Widget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(140, 320, 65, 25));
        label_2->setMinimumSize(QSize(65, 25));
        label_2->setMaximumSize(QSize(65, 25));
        label_2->setFont(font2);
        password = new QLineEdit(Widget);
        password->setObjectName(QString::fromUtf8("password"));
        password->setGeometry(QRect(220, 320, 150, 25));
        password->setMinimumSize(QSize(150, 25));
        password->setMaximumSize(QSize(150, 25));
        password->setFont(font3);

        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QCoreApplication::translate("Widget", "Widget", nullptr));
        background->setText(QString());
        registered->setText(QCoreApplication::translate("Widget", "\346\263\250\345\206\214\350\264\246\345\217\267", nullptr));
        sure->setText(QCoreApplication::translate("Widget", "\347\231\273  \345\275\225", nullptr));
        label->setText(QCoreApplication::translate("Widget", "\347\224\250\346\210\267\345\220\215:", nullptr));
        user->setText(QString());
        winTitle->setText(QCoreApplication::translate("Widget", "Smiley face", nullptr));
        label_2->setText(QCoreApplication::translate("Widget", " \345\257\206   \347\240\201:", nullptr));
        password->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
