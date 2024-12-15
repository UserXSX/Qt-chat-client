#include "chatroom.h"
#include "ui_chatroom.h"

ChatRoom::ChatRoom(QTcpSocket *socket, QString *addr, quint16 *port, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatRoom),
    clientSocket(socket),
    serverAddr(addr),
    serverPort(port)
{
    ui->setupUi(this);
    setWindowFlag(Qt::Window);  // 设置为独立窗口
}

ChatRoom::~ChatRoom()
{
    delete ui;
}

/*
 * 建立和客户端的TCP长连接
 */
void ChatRoom::setUpConnection() {

}

/*
 * 关闭聊天室窗口，并返回至登录窗口
 */
void ChatRoom::back2login() {
    QWidget *logInWindow = this->parentWidget();
    if (logInWindow) {
        logInWindow->show();
    }
    clientSocket->close();
    this->close();
    delete this;
}

/*
 * 点击登出按钮后，提示用户确认是否登出
 */
void ChatRoom::on_logOutButton_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "登出确认", "是否确认登出",
                                  QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        back2login();
    }
}

/*
 * 点击发送按钮后，发送输入框中的消息
 */
void ChatRoom::on_pushButton_clicked()
{

}
