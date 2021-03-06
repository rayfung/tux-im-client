#include "personalcenter.h"
#include "ui_personalcenter.h"
#include "imapi.h"
#include "utils.h"
#include <QString>
#include <QInputDialog>
#include <QMessageBox>

PersonalCenter::PersonalCenter(UserProfile *userProfile, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PersonalCenter)
{
    ui->setupUi(this);
    this->setFixedSize(this->size());
    this->userProfile = userProfile;

    ui->labelNickName->setText(this->userProfile->name);
    ui->lineEditNickName->setText(this->userProfile->name);
    ui->lineEditAccount->setText(QString::number(this->userProfile->account));
    ui->lineEditArea->setText(this->userProfile->area);
    ui->comboBoxSex->setCurrentIndex(this->userProfile->sex == "m" ? 0 : 1);
}

PersonalCenter::~PersonalCenter()
{
    delete ui;
}

void PersonalCenter::on_pushButtonSave_clicked()
{
    QString     nickName;
    QString     area;
    QString     sex;
    QTcpSocket  *tcpSocket;
    IMAPI       imAPI;

    nickName = ui->lineEditNickName->text();
    area     = ui->lineEditArea->text();
    sex      = ui->comboBoxSex->currentIndex() == 0 ? "m" : "f";
    tcpSocket = Utils::getInstance()->getTcpSocket();
    if(!imAPI.modifyUserMessage(tcpSocket, nickName, sex, area))
    {
        QMessageBox::information(this, "提示", "网络不稳定，请稍后再试!");
        return;
    }
    ui->labelNickName->setText(nickName);
    QMessageBox::information(this, "提示", "信息修改成功!");
    emit modifyMessage(nickName);
}

void PersonalCenter::on_pushButtonCancel_clicked()
{
    this->close();
}

void PersonalCenter::on_pushButtonPwdChange_clicked()
{
    bool        ok;
    QString     pwd;
    QString     pwd2;
    QTcpSocket *tcpSocket;
    IMAPI       imAPI;

    pwd = QInputDialog::getText(NULL, "修改密码", "请输入当前密码：", QLineEdit::Password,
                                QString(""), &ok);
    if(!ok)
        return;
    if(pwd != userProfile->pwd)
    {
        QMessageBox::information(this, "提示", "密码错误");
        return;
    }

    pwd = QInputDialog::getText(NULL, "修改密码", "请输入新密码：", QLineEdit::Password,
                                QString(""), &ok);
    if(!ok)
        return;
    if(pwd.length() < 6 || pwd.length() > 20)
    {
        QMessageBox::information(this, "提示", "密码长度必须在 6 到 20 之间");
        return;
    }

    pwd2 = QInputDialog::getText(NULL, "修改密码", "请确认新密码：", QLineEdit::Password,
                                 QString(""), &ok);
    if(!ok)
        return;
    if(pwd != pwd2)
    {
        QMessageBox::information(this, "提示", "两次输入的密码不一致，请重试!");
        return;
    }

    tcpSocket = Utils::getInstance()->getTcpSocket();
    if(imAPI.modifyPwd(tcpSocket, pwd))
    {
        this->userProfile->pwd = pwd;
        QMessageBox::information(this, "提示", "密码修改成功!");
    }
    else
        QMessageBox::information(this, "提示", "网络不稳定，请稍后再试!");
}
