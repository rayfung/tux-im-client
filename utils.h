#ifndef UTILS_H
#define UTILS_H

#include <QTcpSocket>

class Utils
{
public:
    static Utils *getInstance();
    QTcpSocket   *getTcpSocket();

private:
    Utils();

    QTcpSocket   *tcpSocket;
    static Utils *utils;
};

#endif // UTILS_H
