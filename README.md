# Qt-chat-client（未完成）
Qt聊天室的客户端

在`chat_client/widget.cpp`中配置服务器的IP地址和监听端口号：

```c++
    // 连接准备
    clientSocket = new QTcpSocket(this);
    serverAddr = new QString("xxx.xxx.xxx.xxx");  // 填写服务器 IP 地址
    serverPort = new quint16(8080);  // 填写服务器监听的端口号
```

## 客户端请求

### 1.注册（短连接）

客户端发送形如`SignUp [uid]?[password]?[user_name]`的注册请求给服务器，服务器返回注册结果，形如`Pass`（注册成功）和`UidExist`（账号已存在）。

注册成功后，服务器将**User表**中该用户的`online`字段值设置为`0`。

### 2.修改密码（短连接）

客户端发送形如`ChangePwd [uid]?[old password]?[new password]`的修改密码请求给服务器，服务器返回修改结果，形如`Pass`（修改成功）、`NoUid`（账号不存在）和 `WrongPwd`（原密码错误）。

### 3.登录（短连接）

客户端发送形如`LogIn [uid]?[password]`的登录请求给服务器，服务器返回登录结果，形如 `Pass`（登录成功）、`NoUid`（账号不存在） 和 `WrongPwd`（账号密码不匹配）。其中`[uid]`代表用户输入的账号，`[password]`代表密码，下同。

登录成功后，服务器将**User表**中该用户的`online`字段值设置为`1`，表示该用户在线，并且记录系统在线人数的计数器`+1`。

>※ 在登录成功后，客户端发出新的连接请求，该连接为**长连接**，后续聊天室的功能都在此长连接的基础上实现，**直到客户端发出登出请求**。

### 4.登出

客户端发送形如`LogOut [uid]`的登出请求给服务器，服务器断开连接，并将**User表**中该用户的`online`字段值设置为`0`。

### 5.向聊天室中发送消息

客户端向服务器发送形如`Send [message]`的消息转发请求，服务器将`Send [message]`整体转发至所有已连接的用户，其中`[message]`为用户请求转发的消息。`[message]`中**已经包含**用户信息和消息结束符，服务器无需处理。

### 6.显示用户关注列表中的在线用户

客户端向服务器发送形如`FollowingOnline [my_id]`，服务器通过**User表**和**Following表**查找该用户关注的对象中，在线对象的`uid`和`name`，返回的消息形如`FollowingOnline [uid_1]#[name_1] [uid_2]#[name_2]\0`，其中`[uid_1]#[name_1]`为用字符`'#'`分隔的一个关注对象的`uid`和`name`，每个关注对象之间用`空格`分隔，消息结尾处添加结束符`'\0'`。

> **※ 当用户添加关注对象或者取消关注成功后，服务器都需要返回用户关注列表中的在线用户。**

### 7.添加关注对象

客户端向服务器发送形如`Follow [my_id]?[following_id]`，服务器将记录`(my_id, following_id)`插入到**Following表**中，若插入失败，即**User表**中不存在`[following_id]`（不满足外码约束），则返回`NoUid \0`（账号不存在），注意`NoUid`后包含空格，消息结尾处添加结束符`'\0'`；如果插入的记录已存在，服务器不返回消息。

### 8.取消关注

客户端向服务器发送形如`Unfollow [my_id]?[following_id]`，服务器将记录`(my_id, following_id)`从**Following表**中删除。~~如果要删除的记录不存在，服务器不返回消息~~（在不考虑多设备登录的情况下，客户端可以确保这条记录存在）。

### 显示聊天室中的在线人数

...

## 数据库设计

### User表

存储用户的信息，主码为 `uid`

| uid  | password | name | online | img_src |
| ---- | -------- | ---- | ------ | ------- |
|      |          |      |        |         |

- `uid`：用户的账号（数字组合）
- `password`：用户的密码（字母和数字的组合）
- `name`：用户名，注册时添加
- `online`：用于表示用户是否在线，1表示在线，0表示离线
- `img_src`：用户的头像地址，注册时设置为空

### Following表

表示用户对其他用户的“关注”关系，主码为 `(uid, following_uid)`

| uid  | following_uid |
| ---- | ------------- |
|      |               |

- `uid`：关注者的uid
- `following_uid`：被关注者的uid
