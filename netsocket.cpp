#include "netsocket.h"
#include <QDataStream>

NetSocket::NetSocket()
{
}

bool NetSocket::readData(QTcpSocket *tcpSocket, QByteArray &bytes)
{
    if(tcpSocket == NULL)
        return false;

    QDataStream in(tcpSocket);
    quint32     len;

    while(tcpSocket->bytesAvailable() < (int)sizeof(quint32))
    {
        if(!tcpSocket->waitForReadyRead())
            return false;
    }
    in >> len;
    if(len == 0)
        return false;
    while(tcpSocket->bytesAvailable() < len)
    {
        if(!tcpSocket->waitForReadyRead())
            return false;
    }
    bytes = tcpSocket->read(len);

    return true;
}

bool NetSocket::readData(QTcpSocket *tcpSocket)
{
    if(tcpSocket == NULL)
        return false;

    QDataStream in(tcpSocket);
    quint32     len;

    while(tcpSocket->bytesAvailable() < (int)sizeof(quint32))
    {
        if(!tcpSocket->waitForReadyRead())
            return false;
    }
    in >> len;
    if(len != 0)
        return false;

    return true;
}

void NetSocket::sendData(QTcpSocket *tcpSocket, QByteArray bytes)
{
    quint32     len;
    QByteArray  data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_6);

    if(tcpSocket == NULL)
        return;
    len = bytes.length();
    out << len;
    data += bytes;
    tcpSocket->write(data);
}
