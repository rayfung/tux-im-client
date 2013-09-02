#ifndef REGISTRATION_H
#define REGISTRATION_H

#include <QDialog>

#include "datatype.h"

namespace Ui {
    class Registration;
}

class Registration : public QDialog
{
    Q_OBJECT

public:
    explicit Registration(QWidget *parent = 0);
    ~Registration();

private:
    Ui::Registration *ui;

    UserMessage *userMessage;

private slots:
    void on_cancelPushButton_clicked();
    void on_registerPushButton_clicked();
};

#endif // REGISTRATION_H
