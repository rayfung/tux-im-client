#ifndef NETSOCKET_H
#define NETSOCKET_H

#include <QTcpSocket>

class NetSocket
{
public:
    NetSocket();
    void sendData(QTcpSocket *tcpSocket, QByteArray bytes);
    bool readData(QTcpSocket *tcpSocket, QByteArray &bytes);
    bool readData(QTcpSocket *tcpSocket);
};

#endif // NETSOCKET_H
