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
    userMessage = new UserMessage;
}

Registration::~Registration()
{
    delete userMessage;
    delete ui;
}

void Registration::on_registerPushButton_clicked()
{
    IMAPI       imAPI;
    QTcpSocket  *tcpSocket;

    if(ui->nickNameLineEdit->text().trimmed().isEmpty()
       || ui->pwdLineEdit->text().trimmed().isEmpty()
       || ui->areaLineEdit->text().trimmed().isEmpty())
    {
        QMessageBox::warning(this, "提示", "请填写完整信息!!!");
        return;
    }
    this->userMessage->name = ui->nickNameLineEdit->text();
    this->userMessage->pwd  = ui->pwdLineEdit->text();
    this->userMessage->sex  = (ui->sexComboBox->currentIndex()) ? "f" : "m";
    this->userMessage->area = ui->areaLineEdit->text();
    tcpSocket = Utils::getInstance()->getTcpSocket();
    if(!imAPI.registration(tcpSocket, this->userMessage->pwd,
                          this->userMessage->name, this->userMessage->sex,
                          this->userMessage->area, this->userMessage->account))
    {
        QMessageBox::information(this, "提示", "网络不稳定，请稍后再试!");
    } else {
        QMessageBox::information(this, "提示", QString("注册成功！\n你的账号是: %1")
                                 .arg(this->userMessage->account));
    }
    this->close();
}

void Registration::on_cancelPushButton_clicked()
{
    this->close();
}
