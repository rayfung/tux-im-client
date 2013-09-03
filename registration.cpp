#include "registration.h"
#include "ui_registration.h"
#include "imapi.h"
#include "utils.h"
#include <QMessageBox>
#include <QTcpSocket>

Registration::Registration(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Registration)
{
    ui->setupUi(this);
    this->setFixedSize(this->size());
    userProfile = new UserProfile;
}

Registration::~Registration()
{
    delete userProfile;
    delete ui;
}

void Registration::on_registerPushButton_clicked()
{
    IMAPI       imAPI;
    QTcpSocket  *tcpSocket;

    if(ui->nickNameLineEdit->text().trimmed().isEmpty()
       || ui->pwdLineEdit->text().isEmpty()
       || ui->areaLineEdit->text().trimmed().isEmpty())
    {
        QMessageBox::warning(this, "提示", "请填写完整信息!");
        return;
    }
    if(ui->lineEditConfirm->text() != ui->pwdLineEdit->text())
    {
        QMessageBox::warning(this, "提示", "两次输入的密码不一致");
        ui->pwdLineEdit->setFocus();
        ui->pwdLineEdit->selectAll();
        return;
    }
    this->userProfile->name = ui->nickNameLineEdit->text();
    this->userProfile->pwd  = ui->pwdLineEdit->text();
    this->userProfile->sex  = (ui->sexComboBox->currentIndex() ? "f" : "m");
    this->userProfile->area = ui->areaLineEdit->text();
    tcpSocket = Utils::getInstance()->getTcpSocket();
    if(!imAPI.registration(tcpSocket, this->userProfile->pwd,
                          this->userProfile->name, this->userProfile->sex,
                          this->userProfile->area, this->userProfile->account))
    {
        QMessageBox::information(this, "提示", "网络不稳定，请稍后再试!");
    } else {
        QMessageBox::information(this, "提示", QString("注册成功！\n你的账号是: %1")
                                 .arg(this->userProfile->account));
    }
    this->close();
}

void Registration::on_cancelPushButton_clicked()
{
    this->close();
}
