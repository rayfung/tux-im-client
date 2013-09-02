#ifndef USERINFORMATION_H
#define USERINFORMATION_H

#include <QWidget>
#include "datatype.h"

namespace Ui {
class UserInformation;
}

class UserInformation : public QWidget
{
    Q_OBJECT
    
public:
    explicit UserInformation(FriendMessage profile, QWidget *parent = 0);
    ~UserInformation();
    
private:
    Ui::UserInformation *ui;
    FriendMessage friendProfile;
};

#endif // USERINFORMATION_H
