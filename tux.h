#ifndef TUX_H
#define TUX_H

#include <QWidget>
#include <QPoint>
#include <QMenu>
#include <QAction>
#include <QVector>
#include <QTimer>
#include <QSystemTrayIcon>
#include <QCloseEvent>
#include <QListWidgetItem>
#include <QMap>

#include "datatype.h"
#include "imapi.h"
#include "chatwindow.h"

namespace Ui {
class Tux;
}

class Tux : public QWidget
{
    Q_OBJECT
    
public:
    explicit Tux(UserMessage userMessage, QWidget *parent = 0);
    ~Tux();

private:
    void setupMenu();
    int getSelectedRow();
    ChatWindow *getChatWindow(FriendMessage info);
    void closeEvent(QCloseEvent *e);

private slots:
    void on_pushButtonPersonal_clicked();
    void popupMenu(QPoint point);
    void refreshFriendList();
    void askForQuit();
    void showNewNickName(QString nickName);
    void actionShowProfileTriggered();
    void actionDeleteFriendTriggered();
    void actionModDisplayNameTriggered();
    void on_findButton_clicked();
    void on_lineEditSearch_textEdited(const QString &text);
    void on_listWidgetFriend_itemDoubleClicked(QListWidgetItem *item);
    void trayIconActivated(QSystemTrayIcon::ActivationReason reason);

private:
    Ui::Tux *ui;
    QMenu *menu;
    QAction *actionRefresh;
    QAction *actionShowFriendProfile;
    QAction *actionModifyFriendDisplayName;
    QAction *actionDeleteFriend;
    UserMessage userMsg;
    QVector<FriendMessage> friendList;
    IMAPI api;
    QTimer timer;
    QSystemTrayIcon *trayIcon;
    QMenu *trayMenu;
    QAction *actionShowOrHide;
    QAction *actionQuit;
    QMap<quint32, ChatWindow *> chatWindowMap;
};

#endif
