#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    // 账号为 1~12 位数字
    ui->inputAccount->setValidator(new QRegExpValidator(QRegExp("[0-9]{1,12}$")));
    // 密码为 1~12 位字母和数字的组合
    ui->inputPwd->setValidator(new QRegExpValidator(QRegExp("[a-zA-Z0-9]{1,12}$")));

    // 连接准备
    clientSocket = new QTcpSocket(this);
    serverAddr = new QString("159.75.88.12");
    serverPort = new quint16(8888);

    // 连接信号和槽
    connect(clientSocket, &QTcpSocket::readyRead, this, &Widget::readMessage);
    connect(clientSocket, &QTcpSocket::disconnected, this, &Widget::disconnectSlot);
}

Widget::~Widget()
{
    delete ui;
    delete clientSocket;
    delete serverAddr;
    delete serverPort;
}

/*
 * 点击 登录 按钮后的事件
 */
void Widget::on_loginButton_clicked()
{
    // 检查账号是否为空
    QString account = ui->inputAccount->text();
    if (account.isEmpty()) {
        QMessageBox::warning(this, "登录失败", "账号不能为空！");
        return;
    }
    // 检查密码是否为空
    QString password = ui->inputPwd->text();
    if (password.isEmpty()) {
        QMessageBox::warning(this, "登录失败", "密码不能为空！");
        return;
    }

    /*===============发送登录请求，处理响应===============*/
    // 1.建立 TCP 连接
    clientSocket->connectToHost(*serverAddr, *serverPort);
    if (clientSocket->waitForConnected(3000)) {  // 最多等待 3 秒
        // 2.连接成功，准备登录请求
        QString loginRequest = "LogIn " + account + '?' + password;

        if (clientSocket->isOpen() && clientSocket->isValid()) {  // 检查套接字是否连接且有效
            // 3.发送账号和密码给服务器验证
            clientSocket->write(loginRequest.toUtf8());
        } else {  // 请求发送失败
            QMessageBox::critical(this, "请求发送失败", "请求未发送");
            clientSocket->close();
            return;
        }

        // 4.等待服务器的验证响应
        if (clientSocket->waitForReadyRead(3000)) {  // 最多等待 3 秒
            // 5.读取服务器响应
            QByteArray response = clientSocket->readAll();

            // 6.处理响应
            QString res_str = QString::fromUtf8(response);
            if (res_str == "Pass") {  // 登录成功
                // 登录成功，跳转至聊天室界面
                QMessageBox::information(this, "登录成功", "登录成功");
                // 跳转功能未实现...
                clientSocket->close();
                return;

            } else if (res_str == "NoUid") {  // 账号不存在
                QMessageBox::warning(this, "登录失败", "账号不存在");
                clientSocket->close();
                return;
            } else if (res_str == "WrongPwd") {  // 账号密码不匹配
                QMessageBox::warning(this, "登录失败", "账号密码不匹配");
                clientSocket->close();
                return;
            }
        } else {  // 响应超时
            QMessageBox::critical(this, "响应超时", "服务器未响应");
            clientSocket->close();
            return;
        }

    } else {  // 连接失败
        QMessageBox::critical(this, "连接失败", clientSocket->errorString());
        // 关闭套接字，此时会发送 disconnected 信号
        clientSocket->close();
        return;
    }
}

void Widget::readMessage() {

}

void Widget::disconnectSlot() {

}

/*
 * 点击前往注册按钮后的事件
 */
void Widget::on_signupButton_clicked()
{
    SignUp *signUpForm = new SignUp(clientSocket,
                                    serverAddr,
                                    serverPort,
                                    this);
    signUpForm->show();  // 显示注册窗口
    this->hide();  // 隐藏登录窗口
}
