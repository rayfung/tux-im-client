#include "tux.h"
#include "ui_tux.h"
#include "userinformation.h"
#include "utils.h"
#include <QInputDialog>

Tux::Tux(UserMessage userMessage, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Tux),
    userMsg(userMessage)
{
    ui->setupUi(this);
    ui->UserNameLabel->setText(userMsg.name);

    api.getUserList(Utils::getInstance()->getTcpSocket(), friendList);
    showFriendList();

    setupMenu();
    connect(ui->listWidgetFriend, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(popupMenu(QPoint)));
}

Tux::~Tux()
{
    delete ui;
}

void Tux::showFriendList()
{
    for(int i = 0; i < friendList.size(); ++i)
    {
        QString name;

        if(friendList.at(i).displayName.isEmpty())
            name = friendList.at(i).nickName;
        else
            name = friendList.at(i).displayName + "(" + friendList.at(i).nickName + ")";
        ui->listWidgetFriend->addItem(name);
    }
}

void Tux::setupMenu()
{
    menu = new QMenu(this);
    actionShowFriendProfile = menu->addAction("显示好友资料");
    actionModifyFriendDisplayName = menu->addAction("修改好友备注");
    actionDeleteFriend = menu->addAction("删除好友");

    connect(actionDeleteFriend, SIGNAL(triggered()), this, SLOT(actionDeleteFriendTriggered()));
}

void Tux::popupMenu(QPoint point)
{
    menu->popup(ui->listWidgetFriend->mapToGlobal(point));
}

void Tux::actionDeleteFriendTriggered()
{
    int index;

    index = ui->listWidgetFriend->currentRow();
    if(index < 0)
        return;
    if(api.deleteFriend(Utils::getInstance()->getTcpSocket(), friendList.at(index).account))
    {
        friendList.remove(index);
        delete ui->listWidgetFriend->takeItem(index);
    }
}

void Tux::on_findButton_clicked()
{
    bool ok;
    quint32 uid;

    uid = QInputDialog::getInt(this, "查询用户", "请输入用户账号：", 0, 0, 99887766, 1, &ok);
    if(ok)
    {
        UserInformation *userInformation;

        userInformation = new UserInformation();
        userInformation->setAttribute(Qt::WA_DeleteOnClose);
        userInformation->show();
    }
}
