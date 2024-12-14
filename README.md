# Qt-chat-client（未完成）
Qt聊天室的客户端

## 客户端请求

### 登录

客户端发送形如"`LogIn [uid]?[password]`"的登录请求给服务器，服务器返回登录结果，形如 "`Pass`"（登录成功）、"`NoUid`"（账号不存在） 和 "`WrongPwd`"（账号密码不匹配）。其中`[uid]`代表用户输入的账号，`[password]`代表密码，下同。

### 注册

客户端发送形如"`SignUp [uid]?[password]`"的注册请求给服务器，服务器返回注册结果，形如"`Pass`"（注册成功）和"`UidExist`"（账号已存在）。

### 修改密码

客户端发送形如"`ChangePwd [uid]?[old password]?[new password]`"的修改密码请求给服务器，服务器返回修改结果，形如"`Pass`"（修改成功）、"`NoUid`" （账号不存在）和 "`WrongPwd`"（原密码错误）。

### 显示聊天室中的在线人数

...

### 添加关注对象

...

### 显示用户关注列表中的在线用户

...

### 向聊天室中发送消息

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

### Followers表

表示用户对其他用户的“关注”关系，主码为 `(uid, followed_uid)`

| uid  | followed_uid |
| ---- | ------------ |
|      |              |

- `uid`：关注者的uid
- `followed_uid`：被关注者的uid
