#include "userinformation.h"
#include "ui_userinformation.h"

UserInformation::UserInformation(FriendMessage profile, QWidget *parent) :
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
}

UserInformation::~UserInformation()
{
    delete ui;
}
