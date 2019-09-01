/********************************************************************************
** Form generated from reading UI file 'addfriend.ui'
**
** Created by: Qt User Interface Compiler version 5.13.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADDFRIEND_H
#define UI_ADDFRIEND_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_addFriend
{
public:
    QLabel *background;
    QLabel *label_2;
    QPushButton *addButton;
    QLabel *tip;
    QLineEdit *lineEdit;
    QLabel *label;

    void setupUi(QWidget *addFriend)
    {
        if (addFriend->objectName().isEmpty())
            addFriend->setObjectName(QString::fromUtf8("addFriend"));
        addFriend->resize(400, 200);
        addFriend->setMinimumSize(QSize(400, 200));
        addFriend->setMaximumSize(QSize(400, 200));
        background = new QLabel(addFriend);
        background->setObjectName(QString::fromUtf8("background"));
        background->setGeometry(QRect(0, 0, 401, 201));
        label_2 = new QLabel(addFriend);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(20, 50, 181, 31));
        QFont font;
        font.setFamily(QString::fromUtf8("\345\215\216\346\226\207\346\245\267\344\275\223"));
        font.setPointSize(12);
        label_2->setFont(font);
        addButton = new QPushButton(addFriend);
        addButton->setObjectName(QString::fromUtf8("addButton"));
        addButton->setGeometry(QRect(20, 110, 361, 31));
        QFont font1;
        font1.setFamily(QString::fromUtf8("\345\215\216\346\226\207\346\245\267\344\275\223"));
        font1.setPointSize(11);
        addButton->setFont(font1);
        tip = new QLabel(addFriend);
        tip->setObjectName(QString::fromUtf8("tip"));
        tip->setGeometry(QRect(20, 140, 361, 31));
        tip->setFont(font1);
        lineEdit = new QLineEdit(addFriend);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(190, 50, 191, 31));
        QFont font2;
        font2.setFamily(QString::fromUtf8("Arial"));
        font2.setPointSize(12);
        lineEdit->setFont(font2);
        label = new QLabel(addFriend);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 0, 71, 21));
        QFont font3;
        font3.setFamily(QString::fromUtf8("\350\277\267\344\275\240\347\256\200\345\256\266\344\271\246"));
        label->setFont(font3);

        retranslateUi(addFriend);

        QMetaObject::connectSlotsByName(addFriend);
    } // setupUi

    void retranslateUi(QWidget *addFriend)
    {
        addFriend->setWindowTitle(QCoreApplication::translate("addFriend", "Form", nullptr));
        background->setText(QString());
        label_2->setText(QCoreApplication::translate("addFriend", "\345\234\250\350\277\231\351\207\214\350\276\223\345\205\245\347\224\250\346\210\267\345\220\215\357\274\232", nullptr));
        addButton->setText(QCoreApplication::translate("addFriend", "\346\267\273    \345\212\240", nullptr));
        tip->setText(QCoreApplication::translate("addFriend", "\346\217\220\347\244\272:", nullptr));
        label->setText(QCoreApplication::translate("addFriend", "\346\267\273\345\212\240\345\245\275\345\217\213", nullptr));
    } // retranslateUi

};

namespace Ui {
    class addFriend: public Ui_addFriend {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADDFRIEND_H
