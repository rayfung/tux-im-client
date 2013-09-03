#include "tux.h"
#include "ui_tux.h"
#include "userinformation.h"
#include "utils.h"
#include "personalcenter.h"
#include <QInputDialog>
#include <QMessageBox>

Tux::Tux(UserMessage userMessage, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Tux),
    userMsg(userMessage)
{
    ui->setupUi(this);

    ui->UserNameLabel->setText(userMsg.name);

    timer.setInterval(2 * 60 * 1000);
    refreshFriendList();
    setupMenu();
    connect(ui->listWidgetFriend, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(popupMenu(QPoint)));
    connect(&timer, SIGNAL(timeout()), this, SLOT(refreshFriendList()));

    //timer.start();
}

Tux::~Tux()
{
    delete ui;
}

void Tux::closeEvent(QCloseEvent *e)
{
    e->ignore();
    hide();
    actionShowOrHide->setChecked(false);
}

void Tux::askForQuit()
{
    if(QMessageBox::question(this, "询问", "你确定要退出吗？",
                             QMessageBox::Ok | QMessageBox::Cancel,
                             QMessageBox::Cancel)
            == QMessageBox::Ok)
    {
        Utils::getInstance()->getTcpSocket()->close();
        QTimer::singleShot(0, qApp, SLOT(quit()));
    }
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
            name = friendList.at(i).displayName + " (" + friendList.at(i).nickName + ")";
        ui->listWidgetFriend->addItem(name);
        if(friendList.at(i).isOnLine)
            ui->listWidgetFriend->item(i)->setTextColor(Qt::black);
        else
            ui->listWidgetFriend->item(i)->setTextColor(Qt::gray);

        ChatWindow *chat;
        chat = getChatWindow(friendList.at(i));
        chat->setUserInfo(userMsg);
        chat->setFriendInfo(friendList.at(i));
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

    trayMenu = new QMenu(this);
    actionShowOrHide = trayMenu->addAction("显示主窗口");
    actionShowOrHide->setCheckable(true);
    actionShowOrHide->setChecked(true);
    trayMenu->addSeparator();
    actionQuit = trayMenu->addAction("退出");
    connect(actionShowOrHide, SIGNAL(triggered(bool)), this, SLOT(setVisible(bool)));
    connect(actionQuit, SIGNAL(triggered()), this, SLOT(askForQuit()));

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/new/images/app.png"));
    trayIcon->setContextMenu(trayMenu);
    trayIcon->show();
}

void Tux::popupMenu(QPoint point)
{
    menu->popup(ui->listWidgetFriend->mapToGlobal(point));
}

int Tux::getSelectedRow()
{
    int index;
    QList<QListWidgetItem *> list;

    list = ui->listWidgetFriend->selectedItems();
    if(list.size() != 1)
        return -1;
    index = ui->listWidgetFriend->row(list.at(0));
    return index;
}

void Tux::actionDeleteFriendTriggered()
{
    int index;

    index = getSelectedRow();
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

    index = getSelectedRow();
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

    index = getSelectedRow();
    if(index < 0)
        return;

    UserInformation *userInfo = new UserInformation(friendList.at(index), true);

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
        QVector<FriendMessage> list;
        if(api.getUserProfile(Utils::getInstance()->getTcpSocket(), uid, list))
        {
            if(list.size() > 0)
            {
                UserInformation *userInfo = new UserInformation(list.at(0), false);

                connect(userInfo, SIGNAL(newFriend()), this, SLOT(refreshFriendList()));
                userInfo->setAttribute(Qt::WA_DeleteOnClose);
                userInfo->show();
            }
            else
            {
                QMessageBox::information(0, "提示", QString("账号 %1 不存在").arg(uid));
            }
        }
    }
}

void Tux::on_lineEditSearch_textEdited(const QString &text)
{
    int k = -1;

    for(int i = 0; i < friendList.size(); ++i)
    {
        if(friendList.at(i).displayName.contains(text, Qt::CaseInsensitive))
        {
            k = i;
            goto move_selection;
        }
    }

    for(int i = 0; i < friendList.size(); ++i)
    {
        if(friendList.at(i).nickName.contains(text, Qt::CaseInsensitive))
        {
            k = i;
            goto move_selection;
        }
    }

    for(int i = 0; i < friendList.size(); ++i)
    {
        if(QString::number(friendList.at(i).account).contains(text, Qt::CaseInsensitive))
        {
            k = i;
            goto move_selection;
        }
    }

move_selection:
    if(k >= 0)
        ui->listWidgetFriend->setCurrentRow(k);
}

void Tux::on_pushButtonPersonal_clicked()
{
    PersonalCenter  *personalCenter = new PersonalCenter(userMsg);

    connect(personalCenter, SIGNAL(modifyMessage(QString)),
            this, SLOT(showNewNickName(QString)));
    personalCenter->setAttribute(Qt::WA_DeleteOnClose);
    personalCenter->show();
}

void Tux::showNewNickName(QString nickName)
{
    ui->UserNameLabel->setText(nickName);
}

/* 如果对应的窗口不存在，则新建一个窗口 */
ChatWindow *Tux::getChatWindow(FriendMessage info)
{
    quint32 uid = info.account;

    if(!chatWindowMap.contains(uid))
    {
        ChatWindow *chat;

        chat = new ChatWindow(userMsg, info);
        chatWindowMap[uid] = chat;
    }
    return chatWindowMap[uid];
}

void Tux::on_listWidgetFriend_itemDoubleClicked(QListWidgetItem *item)
{
    ChatWindow *chat;
    int index;

    index = ui->listWidgetFriend->row(item);
    if(index < 0)
        return;
    chat = getChatWindow(friendList.at(index));
    chat->show();
    chat->activateWindow();
}
