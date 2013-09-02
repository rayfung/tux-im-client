#ifndef TUX_H
#define TUX_H

#include <QWidget>
#include <QPoint>
#include <QMenu>
#include <QAction>

namespace Ui {
class Tux;
}

class Tux : public QWidget
{
    Q_OBJECT
    
public:
    explicit Tux(QWidget *parent = 0);
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
