#include "tux.h"
#include "ui_tux.h"
#include "userinformation.h"
#include "utils.h"
#include <QInputDialog>
#include <QMessageBox>

Tux::Tux(UserMessage userMessage, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Tux),
    userMsg(userMessage)
{
    ui->setupUi(this);
    ui->UserNameLabel->setText(userMsg.name);

    timer.setInterval(30000);
    refreshFriendList();
    setupMenu();
    connect(ui->listWidgetFriend, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(popupMenu(QPoint)));
    connect(&timer, SIGNAL(timeout()), this, SLOT(refreshFriendList()));

    timer.start();
}

Tux::~Tux()
{
    delete ui;
}

void Tux::refreshFriendList()
{
    friendList.clear();
    api.getUserList(Utils::getInstance()->getTcpSocket(), friendList);
    ui->listWidgetFriend->clear();
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
    actionRefresh = menu->addAction("刷新好友列表");
    actionShowFriendProfile = menu->addAction("显示好友资料");
    actionModifyFriendDisplayName = menu->addAction("修改好友备注");
    actionDeleteFriend = menu->addAction("删除好友");

    connect(actionRefresh, SIGNAL(triggered()), this, SLOT(refreshFriendList()));
    connect(actionShowFriendProfile, SIGNAL(triggered()),
            this, SLOT(actionShowProfileTriggered()));
    connect(actionDeleteFriend, SIGNAL(triggered()), this, SLOT(actionDeleteFriendTriggered()));
    connect(actionModifyFriendDisplayName, SIGNAL(triggered()),
            this, SLOT(actionModDisplayNameTriggered()));
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
    QString name = friendList.at(index).nickName;
    if(!friendList.at(index).displayName.trimmed().isEmpty())
        name = friendList.at(index).displayName;
    if(QMessageBox::question(this, "删除好友", QString("你确定要删除好友 %1？").arg(name),
                             QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Cancel)
            != QMessageBox::Ok)
        return;
    if(api.deleteFriend(Utils::getInstance()->getTcpSocket(), friendList.at(index).account))
    {
        friendList.remove(index);
        delete ui->listWidgetFriend->takeItem(index);
    }
}

void Tux::actionModDisplayNameTriggered()
{
    bool ok;
    QString name;
    int index;

    index = ui->listWidgetFriend->currentRow();
    if(index < 0)
        return;

    name = friendList.at(index).displayName;
    name = QInputDialog::getText(this, "修改好友备注", "请输入备注名称：",
                                 QLineEdit::Normal, name, &ok);
    if(ok)
    {
        if(name.trimmed().isEmpty())
        {
            QMessageBox::information(this, "提示", "备注名称不能为空");
            return;
        }
        if(api.modifyFriendDisplayName(Utils::getInstance()->getTcpSocket(),
                                       friendList.at(index).account, name))
            refreshFriendList();
    }
}

void Tux::actionShowProfileTriggered()
{
    int index;

    index = ui->listWidgetFriend->currentRow();
    if(index < 0)
        return;

    UserInformation *userInfo = new UserInformation(friendList.at(index));

    userInfo->setAttribute(Qt::WA_DeleteOnClose);
    userInfo->show();
}

void Tux::on_findButton_clicked()
{
    bool ok;
    quint32 uid;

    uid = QInputDialog::getInt(NULL, "查询用户", "请输入用户账号：", 0, 0, 99887766, 1, &ok);
    if(ok)
    {
    }
}
