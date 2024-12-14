/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCommandLinkButton>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QLineEdit *inputAccount;
    QLineEdit *inputPwd;
    QPushButton *loginButton;
    QCommandLinkButton *signupButton;
    QLabel *label;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QString::fromUtf8("Widget"));
        Widget->resize(800, 600);
        inputAccount = new QLineEdit(Widget);
        inputAccount->setObjectName(QString::fromUtf8("inputAccount"));
        inputAccount->setGeometry(QRect(260, 180, 300, 50));
        inputAccount->setClearButtonEnabled(true);
        inputPwd = new QLineEdit(Widget);
        inputPwd->setObjectName(QString::fromUtf8("inputPwd"));
        inputPwd->setGeometry(QRect(260, 260, 300, 50));
        inputPwd->setEchoMode(QLineEdit::Password);
        inputPwd->setClearButtonEnabled(true);
        loginButton = new QPushButton(Widget);
        loginButton->setObjectName(QString::fromUtf8("loginButton"));
        loginButton->setGeometry(QRect(260, 350, 300, 50));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        loginButton->setFont(font);
        loginButton->setStyleSheet(QString::fromUtf8("background-color: rgb(46, 103, 211);\n"
"color: rgb(255, 255, 255);"));
        signupButton = new QCommandLinkButton(Widget);
        signupButton->setObjectName(QString::fromUtf8("signupButton"));
        signupButton->setGeometry(QRect(520, 450, 150, 50));
        QFont font1;
        font1.setPointSize(9);
        font1.setUnderline(false);
        signupButton->setFont(font1);
        signupButton->setIconSize(QSize(50, 50));
        label = new QLabel(Widget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(260, 80, 300, 50));
        QFont font2;
        font2.setPointSize(15);
        font2.setBold(true);
        font2.setWeight(75);
        label->setFont(font2);
        label->setAlignment(Qt::AlignCenter);

        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QCoreApplication::translate("Widget", "Chat online", nullptr));
        inputAccount->setPlaceholderText(QCoreApplication::translate("Widget", "\350\257\267\350\276\223\345\205\245\350\264\246\345\217\267\357\274\232", nullptr));
        inputPwd->setText(QString());
        inputPwd->setPlaceholderText(QCoreApplication::translate("Widget", "\350\257\267\350\276\223\345\205\245\345\257\206\347\240\201\357\274\232", nullptr));
        loginButton->setText(QCoreApplication::translate("Widget", "\347\231\273\345\275\225", nullptr));
        signupButton->setText(QCoreApplication::translate("Widget", "\346\263\250\345\206\214\350\264\246\345\217\267", nullptr));
        label->setText(QCoreApplication::translate("Widget", "Chat online", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
