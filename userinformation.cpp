#include "userinformation.h"
#include "ui_userinformation.h"
#include "imapi.h"
#include "utils.h"
#include <QInputDialog>
#include <QMessageBox>

UserInformation::UserInformation(FriendProfile profile, bool isFriend, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserInformation),
    friendProfile(profile)
{
    ui->setupUi(this);
    this->setFixedSize(this->size());
    ui->labelNickName->setText(friendProfile.nickName);
    ui->labelDisplayName->setText(friendProfile.displayName);
    ui->labelGender->setText(friendProfile.sex == QString("f") ? "女" : "男");
    ui->labelUID->setText(QString::number(friendProfile.account));
    ui->labelAddress->setText(friendProfile.area);
    if(isFriend)
        ui->pushButtonMakeFriend->hide();
    else
    {
        ui->labelDisplayName->hide();
        ui->labelDisplayNameTip->hide();
    }
}

UserInformation::~UserInformation()
{
    delete ui;
}

void UserInformation::on_pushButtonMakeFriend_clicked()
{
    IMAPI api;
    QString name;
    bool ok;

    name = QInputDialog::getText(this, "添加好友",
                                 QString("你正在添加 %1 为好友，请输入备注名称")
                                 .arg(friendProfile.nickName), QLineEdit::Normal,
                                 QString(), &ok);
    if(!ok)
        return;
    if(api.addFriend(Utils::getInstance()->getTcpSocket(), friendProfile.account, name))
    {
        ui->pushButtonMakeFriend->hide();
        ui->labelDisplayNameTip->show();
        ui->labelDisplayName->show();
        ui->labelDisplayName->setText(name);
        QMessageBox::information(this, "提示", "添加好友成功");
        emit newFriend();
    }
    else
        QMessageBox::warning(this, "提示", "添加好友失败");
}
