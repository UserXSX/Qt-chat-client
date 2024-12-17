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
    ui->userName->setText("用户名：" + myName);  // 显示登录用户名
    // 要关注的账号为 1~16 位数字
    ui->inputFollowID->setValidator(new QRegExpValidator(QRegExp("[0-9]{1,16}$")));
    setWindowFlag(Qt::Window);  // 设置为独立窗口

    // 将信号和槽连接
    connect(clientSocket, &QTcpSocket::readyRead, this, &ChatRoom::readMessage);
    connect(clientSocket, &QTcpSocket::disconnected, this, &ChatRoom::disconnectSlot);

    // ==>
//    ui->followingList->addItem("id#name");
    // ==>
}

ChatRoom::~ChatRoom()
{
    delete ui;
}

/**
 * @brief 建立和服务器的TCP长连接
 * @return 返回连接是否建立成功
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

/**
 * @brief 捕获聊天室窗口关闭事件，并通过对话框确认用户是否真的想关闭
 * @param event
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

/**
 * @brief 点击登出按钮后，提示用户确认是否登出
 */
void ChatRoom::on_logOutButton_clicked()
{
    this->close();
}

/**
 * @brief 接收消息并做出相应处理
 */
void ChatRoom::readMessage() {
    QByteArray recv = clientSocket->readAll();
    QString recv_str = QString::fromUtf8(recv);
    qDebug() << "buffer: " << recv_str;

    // 按照 '%' 分割字符串，获取每条消息
    QStringList messages = recv_str.split("%", Qt::SkipEmptyParts);

    // 依次处理每条消息
    for (const QString &str : messages) {
        qDebug() << "single message: " << str;
        // 获取消息的关键字，用以处理不同类型的消息
        QString keyword = str.section(' ', 0, 0);
        qDebug() << "keyword: " << keyword << endl;

        if (keyword == "Send") {  // 处理其他用户发送的消息
            // 获取用户发送的内容，即去掉关键字后的部分
            QString content = str.section(' ', 1, -1);
            show_user_send(content);
        } else if (keyword == "FollowingOnline") {  // 获取自己关注列表中的在线用户信息
            // 获取包含关注列表中在线用户的字符串
            QString content = str.section(' ', 1, -1);
            // 将这些在线用户显示到窗口中
            show_online_following(content);
        } else if (keyword == "NoUid") {  // 用户想要关注的账号不存在
            QMessageBox::warning(this, "关注失败", "该账号不存在");
        }
    }
}

/**
 * @brief 显示其他用户发送的消息
 * @param message 其他用户发送的消息
 */
void ChatRoom::show_user_send(QString message) {
    // 首先还原用户发送内容中的'%'
    message.replace("###END###", "%");
    ui->textReceive->append(message);
}

/**
 * @brief 将用户的关注列表中的在线成员以列表的形式呈现到窗口中
 * @param content 包含用户关注列表中的在线成员的字符串
 */
void ChatRoom::show_online_following(QString content) {
    // 先清除所有项
    ui->followingList->clear();
    if (content != "Empty") {  // 用户的关注列表不为空
        // 将所有的用户信息转换成列表的形式
        QStringList users_list = content.split(" ", Qt::SkipEmptyParts);
        // 插入到列表控件中
        ui->followingList->addItems(users_list);
    }
}

void ChatRoom::disconnectSlot() {

}

/**
 * @brief 点击发送按钮后，发送输入框中的消息
 */
void ChatRoom::on_sendButton_clicked()
{
    QString content = ui->textSend->toPlainText();  // 获取输入框中要发送的内容
    if (content.isEmpty()) {
        QMessageBox::information(this, "输入提示", "请先输入内容再发送");
        return;
    }

    // 添加时间戳和用户信息
    QString show_str = QDateTime::currentDateTime().toString("dddd yyyy.MM.dd hh:mm:ss") +
            "\n[" + myID + '#' + myName + "] " + content;

    // 将用户输入内容中的'%'替换成"###END###"，避免被识别为分隔符
    QString replaced_str = show_str;
    replaced_str.replace("%", "###END###");
    QString send_str = "Send " + replaced_str + '%';
    qDebug() << send_str;  // 在控制台输出要发送的请求
    if (clientSocket->isOpen() && clientSocket->isValid()) {
        clientSocket->write(send_str.toUtf8());
        ui->textSend->clear();  // 清空输入框中的内容
        ui->textReceive->append(show_str);
    }
}

/**
 * @brief 点击取消关注按钮后，获取用户选中的关注对象并向服务器发送取消关注请求
 */
void ChatRoom::on_unfollowButton_clicked()
{
    if (ui->followingList->currentItem() == nullptr) {
        QMessageBox::warning(this, "操作提示", "请先选择要取消关注的对象");
    } else {
        // 获取选中的行的内容
        QString selected_following = ui->followingList->currentItem()->text();
        qDebug() << "Selected Row: " << selected_following;
        QStringList splitList = selected_following.split("#");
        followingID = splitList[0];
        followingName = splitList[1];
//        qDebug() << "Selected ID: " << followingID;
//        qDebug() << "Selected Name: " << followingName << endl;
        QString unfollowRequest = "Unfollow " + myID + '?' + followingID;
        qDebug() << unfollowRequest;
        if (clientSocket->isOpen() && clientSocket->isValid()) {
            clientSocket->write(unfollowRequest.toUtf8());
        }
    }
}

/**
 * @brief 点击刷新按钮后，向服务器发送更新数据的请求
 */
void ChatRoom::on_refreshButton_clicked()
{
    // ==>测试刷新按钮的功能
//    QString testStr = "1036#lx 1037#xls 1038#ljx";
//    show_online_following(testStr);
    // ==>

    request_online_following();
}

/**
 * @brief 向服务器请求当前登录账号的关注列表中在线的用户
 */
void ChatRoom::request_online_following() {
    QString online_following_req = "FollowingOnline " + myID;
    qDebug() << online_following_req;
    if (clientSocket->isOpen() && clientSocket->isValid()) {
        clientSocket->write(online_following_req.toUtf8());
    }
}

/**
 * @brief 点击关注按钮后，向服务器发送关注请求
 */
void ChatRoom::on_followButton_clicked()
{
    // 获取单行输入框中的账号
    QString id2follow = ui->inputFollowID->text();
    if (id2follow.isEmpty()) {
        QMessageBox::warning(this, "操作提示", "请先输入要关注的账号");
    } else {
        QString followRequest = "Follow " + myID + '?' + id2follow;
        qDebug() << followRequest;
        if (clientSocket->isOpen() && clientSocket->isValid()) {
            clientSocket->write(followRequest.toUtf8());
        }
    }
}
