/********************************************************************************
** Form generated from reading UI file 'signup.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SIGNUP_H
#define UI_SIGNUP_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SignUp
{
public:
    QLabel *label;
    QPushButton *createButton;
    QLineEdit *inputAccount;
    QLineEdit *inputPwd_1;
    QLineEdit *inputPwd_2;

    void setupUi(QWidget *SignUp)
    {
        if (SignUp->objectName().isEmpty())
            SignUp->setObjectName(QString::fromUtf8("SignUp"));
        SignUp->resize(800, 600);
        label = new QLabel(SignUp);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(160, 80, 500, 50));
        QFont font;
        font.setPointSize(15);
        label->setFont(font);
        label->setAlignment(Qt::AlignCenter);
        createButton = new QPushButton(SignUp);
        createButton->setObjectName(QString::fromUtf8("createButton"));
        createButton->setGeometry(QRect(260, 390, 300, 50));
        QFont font1;
        font1.setBold(true);
        font1.setWeight(75);
        createButton->setFont(font1);
        createButton->setStyleSheet(QString::fromUtf8("background-color: rgb(46, 103, 211);\n"
"color: rgb(255, 255, 255);"));
        inputAccount = new QLineEdit(SignUp);
        inputAccount->setObjectName(QString::fromUtf8("inputAccount"));
        inputAccount->setGeometry(QRect(260, 160, 300, 50));
        inputAccount->setClearButtonEnabled(true);
        inputPwd_1 = new QLineEdit(SignUp);
        inputPwd_1->setObjectName(QString::fromUtf8("inputPwd_1"));
        inputPwd_1->setGeometry(QRect(260, 230, 300, 50));
        inputPwd_1->setEchoMode(QLineEdit::Password);
        inputPwd_1->setClearButtonEnabled(true);
        inputPwd_2 = new QLineEdit(SignUp);
        inputPwd_2->setObjectName(QString::fromUtf8("inputPwd_2"));
        inputPwd_2->setGeometry(QRect(260, 300, 300, 50));
        inputPwd_2->setEchoMode(QLineEdit::Password);
        inputPwd_2->setClearButtonEnabled(true);

        retranslateUi(SignUp);

        QMetaObject::connectSlotsByName(SignUp);
    } // setupUi

    void retranslateUi(QWidget *SignUp)
    {
        SignUp->setWindowTitle(QCoreApplication::translate("SignUp", "Chat online", nullptr));
        label->setText(QCoreApplication::translate("SignUp", "\345\210\233\345\273\272\346\226\260\350\264\246\346\210\267", nullptr));
        createButton->setText(QCoreApplication::translate("SignUp", "\346\263\250\345\206\214", nullptr));
        inputAccount->setPlaceholderText(QCoreApplication::translate("SignUp", "\350\257\267\350\276\223\345\205\245\350\264\246\345\217\267\357\274\232", nullptr));
        inputPwd_1->setPlaceholderText(QCoreApplication::translate("SignUp", "\350\257\267\350\276\223\345\205\245\345\257\206\347\240\201\357\274\232", nullptr));
        inputPwd_2->setPlaceholderText(QCoreApplication::translate("SignUp", "\350\257\267\347\241\256\350\256\244\345\257\206\347\240\201\357\274\232", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SignUp: public Ui_SignUp {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SIGNUP_H
