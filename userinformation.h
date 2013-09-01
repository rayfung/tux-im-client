#ifndef USERINFORMATION_H
#define USERINFORMATION_H

#include <QWidget>

namespace Ui {
class UserInformation;
}

class UserInformation : public QWidget
{
    Q_OBJECT
    
public:
    explicit UserInformation(QWidget *parent = 0);
    ~UserInformation();
    
private:
    Ui::UserInformation *ui;
};

#endif // USERINFORMATION_H
