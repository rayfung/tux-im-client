#include "userinformation.h"
#include "ui_userinformation.h"

UserInformation::UserInformation(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserInformation)
{
    ui->setupUi(this);
}

UserInformation::~UserInformation()
{
    delete ui;
}
