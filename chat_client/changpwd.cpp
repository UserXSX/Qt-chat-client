#include "changpwd.h"
#include "ui_changpwd.h"

ChangPwd::ChangPwd(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChangPwd)
{
    ui->setupUi(this);
}

ChangPwd::~ChangPwd()
{
    delete ui;
}
