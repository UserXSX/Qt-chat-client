#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QRegExpValidator>
#include <QMessageBox>
#include <QTcpSocket>
#include <QString>
#include "signup.h"
#include "chatroom.h"
#include "changepwd.h"


QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_loginButton_clicked();

    void on_signupButton_clicked();

    void on_changePwdBtn_clicked();

    void on_showPwd_stateChanged();

private:
    Ui::Widget *ui;
    QTcpSocket *clientSocket;
    QString *serverAddr;
    quint16 *serverPort;

    void enter_chatRoom(QString uid, QString name);
};
#endif // WIDGET_H
