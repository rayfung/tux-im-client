#include "login.h"
#include "ui_login.h"
#include "registration.h"

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    this->setFixedSize(this->size());
}

Login::~Login()
{
    delete ui;
}

void Login::on_loginPushButton_clicked()
{

}

void Login::on_registerPushButton_clicked()
{
    Registration registration(&userMessage);

    this->hide();
    registration.exec();
    this->show();
}
