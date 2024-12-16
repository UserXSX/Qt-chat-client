#include "chatroom.h"
#include "ui_chatroom.h"

ChatRoom::ChatRoom(QTcpSocket *socket, QString *addr, quint16 *port, QString uid, QString name, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatRoom),
    clientSocket(socket),
    serverAddr(addr),
    serverPort(port),
    myID(uid),
    myName(name)
{
    ui->setupUi(this);
    ui->userName->setText(myName);  // 显示登录用户名
    setWindowFlag(Qt::Window);  // 设置为独立窗口

    // 将信号和槽连接
    connect(clientSocket, &QTcpSocket::readyRead, this, &ChatRoom::readMessage);
    connect(clientSocket, &QTcpSocket::disconnected, this, &ChatRoom::disconnectSlot);
}

ChatRoom::~ChatRoom()
{
    delete ui;
}

/*
 * 建立和服务器的TCP长连接
 */
bool ChatRoom::setUpConnection() {
    clientSocket->connectToHost(*serverAddr, *serverPort);
    if (clientSocket->waitForConnected(3000)) {
        // 连接成功
        return true;
    } else {  // 连接失败
        QMessageBox::critical(this, "进入聊天室失败", clientSocket->errorString());
        return false;
    }
}

/*
 * 捕获聊天室窗口关闭事件，并通过对话框确认用户是否真的想关闭
 * 两种情况：
 * 1.点击右上角的“X”按钮
 * 2.点击聊天室窗口中的“登出”按钮
 */
void ChatRoom::closeEvent(QCloseEvent *event) {
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "登出确认", "是否确认登出",
                                  QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        QWidget *logInWindow = this->parentWidget();
        if (logInWindow) {
            logInWindow->show();
        }

        QString logoutRequest = "LogOut " + myID;
        qDebug() << logoutRequest;
        // 若套接字已连接，则向服务器发送登出请求后将其关闭
        if (clientSocket->state() == QAbstractSocket::ConnectedState) {
            // 发送登出请求
            if (clientSocket->isOpen() && clientSocket->isValid()) {
                clientSocket->write(logoutRequest.toUtf8());
            }

            // 关闭套接字
            clientSocket->close();
        }

        event->accept();  // 接受关闭事件，窗口将会关闭
        delete this;
    } else {
        event->ignore();  // 忽略关闭事件，窗口不会关闭
    }
}

/*
 * 点击登出按钮后，提示用户确认是否登出
 */
void ChatRoom::on_logOutButton_clicked()
{
    this->close();
}

/*
 * 接收消息并做出相应处理
 */
void ChatRoom::readMessage() {
    QByteArray recv = clientSocket->readAll();
    QString recv_str = QString::fromUtf8(recv);
    qDebug() << "buffer: " << recv_str;

    // 按照 '\0' 分割字符串，获取每条消息
    QStringList messages = recv_str.split(QChar('\0'), Qt::SkipEmptyParts);

    // 依次处理每条消息
    for (const QString &str : messages) {
        qDebug() << "receive: " << str;
        // 获取消息的关键字，用以处理不同类型的消息
        QString keyword = str.section(' ', 0, 0);
        qDebug() << "keyword: " << keyword << endl;

        if (keyword == "Send") {  // 处理其他用户发送的消息
            // 获取用户发送的内容，即去掉关键字后的部分
            QString content = str.section(' ', 1, -1);
            show_user_send(content);
        }
    }
}

/*
 * 显示其他用户发送的消息
 */
void ChatRoom::show_user_send(QString message) {
    ui->textReceive->append(message);
}

void ChatRoom::disconnectSlot() {

}

/*
 * 点击发送按钮后，发送输入框中的消息
 */
void ChatRoom::on_sendButton_clicked()
{
    QString content = ui->textSend->toPlainText();  // 获取输入框中要发送的内容

    QString show_str = QDateTime::currentDateTime().toString("dddd yyyy.MM.dd hh:mm:ss") +
            "\n[" + myID + '#' + myName + "] " + content;

    QString send_str = "Send " + show_str + '\0';
    qDebug() << send_str;  // 在控制台输出要发送的请求
    if (clientSocket->isOpen() && clientSocket->isValid()) {
        clientSocket->write(send_str.toUtf8());
        ui->textSend->clear();  // 清空输入框中的内容
        ui->textReceive->append(show_str);
    }
}
