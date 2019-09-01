/********************************************************************************
** Form generated from reading UI file 'contact.ui'
**
** Created by: Qt User Interface Compiler version 5.13.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONTACT_H
#define UI_CONTACT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_contact
{
public:
    QLabel *background;
    QPushButton *tikTok;
    QPushButton *cancel;
    QTextEdit *read;
    QTextEdit *write;
    QPushButton *send;
    QPushButton *sendFile;
    QLabel *username;

    void setupUi(QWidget *contact)
    {
        if (contact->objectName().isEmpty())
            contact->setObjectName(QString::fromUtf8("contact"));
        contact->resize(400, 400);
        contact->setMinimumSize(QSize(400, 400));
        contact->setMaximumSize(QSize(400, 400));
        background = new QLabel(contact);
        background->setObjectName(QString::fromUtf8("background"));
        background->setGeometry(QRect(0, 0, 400, 400));
        background->setMinimumSize(QSize(400, 400));
        background->setMaximumSize(QSize(400, 400));
        tikTok = new QPushButton(contact);
        tikTok->setObjectName(QString::fromUtf8("tikTok"));
        tikTok->setGeometry(QRect(10, 360, 51, 23));
        cancel = new QPushButton(contact);
        cancel->setObjectName(QString::fromUtf8("cancel"));
        cancel->setGeometry(QRect(330, 360, 61, 23));
        read = new QTextEdit(contact);
        read->setObjectName(QString::fromUtf8("read"));
        read->setGeometry(QRect(10, 30, 381, 231));
        read->setReadOnly(true);
        write = new QTextEdit(contact);
        write->setObjectName(QString::fromUtf8("write"));
        write->setGeometry(QRect(10, 270, 381, 81));
        send = new QPushButton(contact);
        send->setObjectName(QString::fromUtf8("send"));
        send->setGeometry(QRect(260, 360, 61, 23));
        sendFile = new QPushButton(contact);
        sendFile->setObjectName(QString::fromUtf8("sendFile"));
        sendFile->setGeometry(QRect(70, 360, 75, 23));
        username = new QLabel(contact);
        username->setObjectName(QString::fromUtf8("username"));
        username->setGeometry(QRect(10, 0, 121, 31));

        retranslateUi(contact);

        QMetaObject::connectSlotsByName(contact);
    } // setupUi

    void retranslateUi(QWidget *contact)
    {
        contact->setWindowTitle(QCoreApplication::translate("contact", "Form", nullptr));
        background->setText(QString());
        tikTok->setText(QCoreApplication::translate("contact", "\346\212\226\344\270\200\346\212\226", nullptr));
        cancel->setText(QCoreApplication::translate("contact", "\345\217\226\346\266\210", nullptr));
        send->setText(QCoreApplication::translate("contact", "\345\217\221\351\200\201", nullptr));
        sendFile->setText(QCoreApplication::translate("contact", "\345\217\221\351\200\201\346\226\207\344\273\266", nullptr));
        username->setText(QCoreApplication::translate("contact", "username", nullptr));
    } // retranslateUi

};

namespace Ui {
    class contact: public Ui_contact {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONTACT_H
