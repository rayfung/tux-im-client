#include "tux.h"
#include "ui_tux.h"
#include "userinformation.h"
#include <QInputDialog>

Tux::Tux(UserMessage userMessage, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Tux)
{
    ui->setupUi(this);
    connect(ui->listWidgetFriend, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(popupMenu(QPoint)));
}

Tux::~Tux()
{
    delete ui;
}

void Tux::setupMenu()
{
    menu = new QMenu(this);
    actionShowFriendProfile = menu->addAction("显示好友资料");
    actionModifyFriendDisplayName = menu->addAction("修改好友备注");
    actionDeleteFriend = menu->addAction("删除好友");
}

void Tux::popupMenu(const QPoint &point)
{
    menu->popup(mapToGlobal(point));
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
