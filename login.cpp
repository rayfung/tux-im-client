#include "login.h"
#include "ui_login.h"
#include "registration.h"

Login::Login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
}

Login::~Login()
{
    delete ui;
}

void Login::on_regButton_clicked()
{
    Registration *reg = new Registration;
    reg->show();
}
