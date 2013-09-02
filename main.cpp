#include <QtGui/QApplication>
#include <QTextCodec>
#include "login.h"
#include "network/datapool.h"
#include "chatwindow.h"
#include "tux.h"
#include "network/server.h"
#include "config.h"

#include <QMessageBox>

DataPool g_dataPool;
Server   g_server;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(false);

    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    Config  config;

    if(!g_server.listen(config.bindPort))
    {
        QMessageBox::critical(NULL, "错误", "网络端口被占用!");
        return 1;
    }

    Login w;

    w.show();
    return a.exec();
}
