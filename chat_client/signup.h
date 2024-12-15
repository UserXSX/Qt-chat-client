#ifndef SIGNUP_H
#define SIGNUP_H

#include <QWidget>
#include <QMessageBox>
#include <QRegExpValidator>
#include <QTcpSocket>
#include <QString>

namespace Ui {
class SignUp;
}

class SignUp : public QWidget
{
    Q_OBJECT

public:
    explicit SignUp(QTcpSocket *socket,
                    QString *addr,
                    quint16 *port,
                    QWidget *parent = nullptr);
    ~SignUp();

private slots:
    void on_createButton_clicked();

    void on_returnButton_clicked();

private:
    Ui::SignUp *ui;
    QTcpSocket *clientSocket;
    QString *serverAddr;
    quint16 *serverPort;
};

#endif // SIGNUP_H
