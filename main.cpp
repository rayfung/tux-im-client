#include <QtGui/QApplication>
#include <QTextCodec>
#include "login.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    Login w;

    w.show();
    return a.exec();
}
