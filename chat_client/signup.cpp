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

    // 账号为 1~16 位数字
    ui->inputAccount->setValidator(new QRegExpValidator(QRegExp("[0-9]{1,16}$")));
    // 密码为 1~16 位字母和数字的组合
    ui->inputPwd_1->setValidator(new QRegExpValidator(QRegExp("[a-zA-Z0-9]{1,16}$")));
    ui->inputPwd_2->setValidator(new QRegExpValidator(QRegExp("[a-zA-Z0-9]{1,16}$")));
    // 用户名为 1~16 位字母和数字的组合
    ui->inputName->setValidator(new QRegExpValidator(QRegExp("[a-zA-Z0-9]{1,16}$")));
}

SignUp::~SignUp()
{
    delete ui;
}

/**
 * @brief 点击注册按钮后，将注册账号所需的信息发送给服务器
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
    // 检查用户名是否为空
    QString user_name = ui->inputName->text();
    if (user_name.isEmpty()) {
        QMessageBox::warning(this, "注册失败", "用户名不能为空！");
        return;
    }
    // 构造注册请求
    QString signUpRequest = "SignUp " + account + '?' + password_1 + '?' + user_name;
    qDebug() << signUpRequest;

    /*===============发送注册请求，处理响应===============*/
    // 1.建立 TCP 连接
    clientSocket->connectToHost(*serverAddr, *serverPort);
    if (clientSocket->waitForConnected(3000)) {  // 最多等待 3 秒

        if (clientSocket->isOpen() && clientSocket->isValid()) {  // 检查套接字是否打开且有效
            // 2.发送账号和密码给服务器验证
            clientSocket->write(signUpRequest.toUtf8());
        } else {  // 请求发送失败
            QMessageBox::critical(this, "请求发送失败", "请求未发送");
            clientSocket->close();
            return;
        }

        // 3.等待服务器的验证响应
        if (clientSocket->waitForReadyRead(3000)) {  // 最多等待 3 秒
            // 4.读取服务器响应
            QByteArray response = clientSocket->readAll();

            // 5.处理响应
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
        return;
    }

}

/**
 * @brief 在注册界面点击登录按钮，返回至登录界面
 */
void SignUp::on_returnButton_clicked()
{
    this->close();  // 关闭注册窗口
}

void SignUp::closeEvent(QCloseEvent *event) {
    QWidget *logInWindow = this->parentWidget();  // 获取父窗口
    if (logInWindow) {
        logInWindow->show();  // 显示登录窗口
    }
    event->accept();
    delete this;  // 手动删除注册窗口以释放内存
}

void SignUp::on_showPwd_stateChanged()
{
    if (ui->showPwd->checkState() == Qt::Checked) {  // 选中时显示密码明文
        ui->inputPwd_1->setEchoMode(QLineEdit::Normal);
        ui->inputPwd_2->setEchoMode(QLineEdit::Normal);
    } else {
        ui->inputPwd_1->setEchoMode(QLineEdit::Password);
        ui->inputPwd_2->setEchoMode(QLineEdit::Password);
    }
}
