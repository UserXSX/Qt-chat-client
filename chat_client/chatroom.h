#ifndef CHATROOM_H
#define CHATROOM_H

#include <QWidget>
#include <QMessageBox>
#include <QRegExpValidator>
#include <QTcpSocket>
#include <QString>

namespace Ui {
class ChatRoom;
}

class ChatRoom : public QWidget
{
    Q_OBJECT

public:
    explicit ChatRoom(QTcpSocket *socket,
                      QString *addr,
                      quint16 *port,
                      QWidget *parent = nullptr);
    ~ChatRoom();

private slots:
    void on_logOutButton_clicked();

    void on_pushButton_clicked();

private:
    Ui::ChatRoom *ui;
    QTcpSocket *clientSocket;
    QString *serverAddr;
    quint16 *serverPort;

    void setUpConnection();

    void back2login();
};

#endif // CHATROOM_H
