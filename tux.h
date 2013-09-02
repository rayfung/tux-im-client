#ifndef TUX_H
#define TUX_H

#include <QWidget>
#include <QPoint>
#include <QMenu>
#include <QAction>
#include <QVector>

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
    void showFriendList();
    
private slots:
    void popupMenu(QPoint point);
    void actionDeleteFriendTriggered();
    void on_findButton_clicked();

private:
    Ui::Tux *ui;
    QMenu *menu;
    QAction *actionShowFriendProfile;
    QAction *actionModifyFriendDisplayName;
    QAction *actionDeleteFriend;
    UserMessage userMsg;
    QVector<FriendMessage> friendList;
    IMAPI api;
};

#endif
