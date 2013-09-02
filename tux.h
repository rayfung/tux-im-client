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

#include "datatype.h"
#include "imapi.h"

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
    void closeEvent(QCloseEvent *e);
    
private slots:
    void popupMenu(QPoint point);
    void refreshFriendList();
    void actionShowProfileTriggered();
    void actionDeleteFriendTriggered();
    void actionModDisplayNameTriggered();
    void on_findButton_clicked();
    void on_lineEditSearch_textEdited(const QString &text);

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
};

#endif
