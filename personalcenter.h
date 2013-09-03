#ifndef PERSONALCENTER_H
#define PERSONALCENTER_H

#include <QDialog>

#include "datatype.h"

namespace Ui {
    class PersonalCenter;
}

class PersonalCenter : public QDialog
{
    Q_OBJECT

public:
    explicit PersonalCenter(UserMessage userMessage, QWidget *parent = 0);
    ~PersonalCenter();

private:
    Ui::PersonalCenter *ui;

    UserMessage userMessage;

private slots:
    void on_pushButtonPwdChange_clicked();
    void on_pushButtonCancel_clicked();
    void on_pushButtonSave_clicked();

signals:
    void modifyMessage(QString nickName);
};

#endif // PERSONALCENTER_H
