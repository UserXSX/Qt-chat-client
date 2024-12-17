#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    // 账号为 1~16 位数字
    ui->inputAccount->setValidator(new QRegExpValidator(QRegExp("[0-9]{1,16}$")));
    // 密码为 1~16 位字母和数字的组合
    ui->inputPwd->setValidator(new QRegExpValidator(QRegExp("[a-zA-Z0-9]{1,16}$")));

    // 连接准备
    clientSocket = new QTcpSocket(this);
    serverAddr = new QString("");  // 填写服务器 IP 地址
    serverPort = new quint16(8080);  // 填写服务器监听的端口号
}

Widget::~Widget()
{
    delete ui;
    delete clientSocket;
    delete serverAddr;
    delete serverPort;
}

/**
 * @brief 点击登录按钮后，将登录请求发送给服务器验证
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

    QString loginRequest = "LogIn " + account + '?' + password;
    qDebug() << loginRequest;

    // ==>跳过服务器登录验证，测试跳转聊天室窗口效果
//    enter_chatRoom(account, "Unknown");
//    return;
    // ==>测试代码结束

    /*===============发送登录请求，处理响应===============*/
    // 1.建立 TCP 连接
    clientSocket->connectToHost(*serverAddr, *serverPort);
    if (clientSocket->waitForConnected(3000)) {  // 最多等待 3 秒
        if (clientSocket->isOpen() && clientSocket->isValid()) {  // 检查套接字是否打开且有效
            // 2.发送账号和密码给服务器验证
            clientSocket->write(loginRequest.toUtf8());
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
            if (res_str == "NoUid") {  // 账号不存在
                QMessageBox::warning(this, "登录失败", "账号不存在");
                clientSocket->close();
                return;
            } else if (res_str == "WrongPwd") {  // 账号密码不匹配
                QMessageBox::warning(this, "登录失败", "账号密码不匹配");
                clientSocket->close();
                return;
            } else {
                QStringList splitList = res_str.split(" ");  // 分割字符串"Pass [user_name]"
                if (splitList.size() == 2 && splitList[0] == "Pass"){
                    // 登录成功
                    QString user_name = splitList[1];
                    clientSocket->close();
                    // 跳转至聊天室界面
                    enter_chatRoom(account, user_name);
                    return;
                }
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
 * @brief 点击注册账号按钮后，从登录界面跳转至注册界面
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

/**
 * @brief 点击修改密码按钮后，从登录界面跳转至修改密码界面
 */
void Widget::on_changePwdBtn_clicked()
{
    ChangePwd *changePwdForm = new ChangePwd(clientSocket,
                                             serverAddr,
                                             serverPort,
                                             this);
    changePwdForm->show();
    this->hide();
}

/**
 * @brief 从登录界面跳转至聊天室界面
 * @param uid 当前登录的用户账号
 * @param name 当前登录的用户名
 */
void Widget::enter_chatRoom(QString uid, QString name) {
    ChatRoom *chatForm = new ChatRoom(clientSocket,
                                      serverAddr,
                                      serverPort,
                                      uid,
                                      name,
                                      this);

    bool connected = chatForm->setUpConnection();

    // ==>跳过聊天室的连接验证，测试聊天室的功能
//    connected = true;
    // ==>测试结束

    if (connected) {
        // 如果连接成功，就显示聊天室窗口，隐藏登录窗口
        chatForm->show();
        this->hide();
    } else {
        // 否则删除已经创建的聊天室窗口，即留在登录窗口
        delete chatForm;
    }
}

/**
 * @brief 根据“显示密码”复选框是否选中来决定密码的显示方式
 */
void Widget::on_showPwd_stateChanged()
{
    if (ui->showPwd->checkState() == Qt::Checked) {  // 选中时显示密码明文
        ui->inputPwd->setEchoMode(QLineEdit::Normal);
    } else {
        ui->inputPwd->setEchoMode(QLineEdit::Password);
    }
}
