#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QCloseEvent>

#include "datatype.h"

namespace Ui {
    class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = 0);
    ~Login();

private:
    void closeEvent(QCloseEvent *e);

private:
    Ui::Login *ui;
    UserMessage    userMessage;

private slots:
    void on_registerPushButton_clicked();
    void on_loginPushButton_clicked();
};

#endif // LOGIN_H
