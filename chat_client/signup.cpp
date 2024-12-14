#include "signup.h"
#include "ui_signup.h"

SignUp::SignUp(QTcpSocket *socket, QString *addr, quint16 *port, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SignUp),
    clientSocket(socket),
    serverAddr(addr),
    serverPort(port)
{
    ui->setupUi(this);
    setWindowFlag(Qt::Window);  // 设置为独立窗口

    // 账号为 1~12 位数字
    ui->inputAccount->setValidator(new QRegExpValidator(QRegExp("[0-9]{1,12}$")));
    // 密码为 1~12 位字母和数字的组合
    ui->inputPwd_1->setValidator(new QRegExpValidator(QRegExp("[a-zA-Z0-9]{1,12}$")));
    ui->inputPwd_2->setValidator(new QRegExpValidator(QRegExp("[a-zA-Z0-9]{1,12}$")));

    qDebug() << *serverAddr << endl;
    qDebug() << *serverPort << endl;
}

SignUp::~SignUp()
{
    delete ui;
}

/*
 * 点击 注册 按钮后的事件
 */
void SignUp::on_createButton_clicked()
{
    // 检查账号是否为空
    QString account = ui->inputAccount->text();
    if (account.isEmpty()) {
        QMessageBox::warning(this, "注册失败", "账号不能为空！");
        return;
    }
    // 检查密码是否为空
    QString password_1 = ui->inputPwd_1->text();
    QString password_2 = ui->inputPwd_2->text();
    if (password_1.isEmpty() || password_2.isEmpty()) {
        QMessageBox::warning(this, "注册失败", "密码不能为空！");
        return;
    }
    // 检查两个密码是否一致
    if (password_1 != password_2) {
        QMessageBox::warning(this, "注册失败", "两次输入的密码不一致！");
        return;
    }

    /*===============发送注册请求，处理响应===============*/
    // 1.建立 TCP 连接
    clientSocket->connectToHost(*serverAddr, *serverPort);
    if (clientSocket->waitForConnected(3000)) {  // 最多等待 3 秒
        // 2.连接成功，准备注册请求
        QString signUpRequest = "SignUp " + account + '?' + password_1;

        if (clientSocket->isOpen() && clientSocket->isValid()) {  // 检查套接字是否连接且有效
            // 3.发送账号和密码给服务器验证
            clientSocket->write(signUpRequest.toUtf8());
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
            if (res_str == "Pass") {  // 注册成功
                // 注册成功，跳转至登录界面
                QMessageBox::information(this, "注册成功", "点击按钮返回至登录界面");
                QWidget *logInWindow = this->parentWidget();  // 获取父窗口
                if (logInWindow) {
                    logInWindow->show();  // 显示登录窗口
                }
                clientSocket->close();
                this->close();  // 关闭注册窗口
                delete this;  // 手动删除注册窗口以释放内存
            } else if (res_str == "UidExist") {  // 账号已存在
                QMessageBox::warning(this, "注册失败", "账号已存在");
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