#include "login.h"
#include "ui_login.h"
#include "registration.h"
#include "tux.h"
#include "utils.h"
#include "imapi.h"
#include "network/server.h"

#include <QTcpSocket>
#include <QMessageBox>
#include <QTimer>

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    this->setFixedSize(this->size());
}

Login::~Login()
{
    delete ui;
}

void Login::closeEvent(QCloseEvent *e)
{
    e->accept();
    QTimer::singleShot(0, qApp, SLOT(quit()));
}

void Login::on_loginPushButton_clicked()
{
    extern Server   g_server;
    Tux         *tux;
    IMAPI       imAPI;
    QTcpSocket *tcpSocket;

    if(ui->accountLineEdit->text().trimmed().isEmpty() ||
       ui->pwdLineEdit->text().isEmpty())
    {
        QMessageBox::information(this, "提示", "请填好账号和密码!");
        return;
    }

    this->userMessage.account = (quint32)ui->accountLineEdit->text().toUInt();
    this->userMessage.pwd = ui->pwdLineEdit->text();
    tcpSocket = Utils::getInstance()->getTcpSocket();
    if(!imAPI.login(tcpSocket, ui->accountLineEdit->text().toUInt(),
                    ui->pwdLineEdit->text(), g_server.getIp(), g_server.getPort(),
                    this->userMessage))
    {
        QMessageBox::information(this, "提示", "账号或密码错误!");
        return;
    }
    tux = new Tux(this->userMessage);
    tux->show();
    this->hide();
}

void Login::on_registerPushButton_clicked()
{
    Registration registration(this);

    registration.exec();
}
