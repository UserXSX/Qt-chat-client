#ifndef CHANGEPWD_H
#define CHANGEPWD_H

#include <QWidget>
#include <QMessageBox>
#include <QRegExpValidator>
#include <QTcpSocket>
#include <QString>
#include <QCloseEvent>

namespace Ui {
class ChangePwd;
}

class ChangePwd : public QWidget
{
    Q_OBJECT

public:
    explicit ChangePwd(QTcpSocket *socket,
                       QString *addr,
                       quint16 *port,
                       QWidget *parent = nullptr);
    ~ChangePwd();

private slots:
    void on_returnButton_clicked();

    void on_submitButton_clicked();

    void on_showPwd_stateChanged();

private:
    Ui::ChangePwd *ui;
    QTcpSocket *clientSocket;
    QString *serverAddr;
    quint16 *serverPort;

    void closeEvent(QCloseEvent *event) override;
};

#endif // CHANGEPWD_H
