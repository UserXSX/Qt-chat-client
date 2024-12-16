#ifndef CHATROOM_H
#define CHATROOM_H

#include <QWidget>
#include <QMessageBox>
#include <QRegExpValidator>
#include <QTcpSocket>
#include <QString>
#include <QDateTime>

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
                      QString uid,
                      QString name,
                      QWidget *parent = nullptr);
    ~ChatRoom();

    bool setUpConnection();

private slots:
    void on_logOutButton_clicked();

    void readMessage();

    void disconnectSlot();

    void on_sendButton_clicked();

private:
    Ui::ChatRoom *ui;
    QTcpSocket *clientSocket;
    QString *serverAddr;
    quint16 *serverPort;
    QString myID;
    QString myName;

    void back2login();

    void closeEvent(QCloseEvent *event) override;

    void show_user_send(QString message);
};

#endif // CHATROOM_H
