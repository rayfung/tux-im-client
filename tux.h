#ifndef TUX_H
#define TUX_H

#include <QWidget>
#include <QPoint>
#include <QMenu>
#include <QAction>

#include "datatype.h"

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
    void popupMenu(const QPoint &point);
    void on_findButton_clicked();

private:
    Ui::Tux *ui;
    QMenu *menu;
    QAction *actionShowFriendProfile;
    QAction *actionModifyFriendDisplayName;
    QAction *actionDeleteFriend;
};

#endif
