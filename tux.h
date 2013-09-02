#ifndef TUX_H
#define TUX_H

#include <QWidget>
#include <QPoint>
#include <QMenu>
#include <QAction>
#include <QVector>
#include <QTimer>

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
    
private slots:
    void popupMenu(QPoint point);
    void refreshFriendList();
    void actionDeleteFriendTriggered();
    void actionModDisplayNameTriggered();
    void on_findButton_clicked();

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
};

#endif
