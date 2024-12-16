#ifndef CHANGPWD_H
#define CHANGPWD_H

#include <QWidget>

namespace Ui {
class ChangPwd;
}

class ChangPwd : public QWidget
{
    Q_OBJECT

public:
    explicit ChangPwd(QWidget *parent = nullptr);
    ~ChangPwd();

private:
    Ui::ChangPwd *ui;
};

#endif // CHANGPWD_H
