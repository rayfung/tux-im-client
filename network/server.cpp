#include "server.h"
#include "datapool.h"

Server::Server(QObject *parent) : QObject(parent)
{
    QObject::connect(&tcpServer, SIGNAL(newConnection()), this,
                     SLOT(newConnection()), Qt::QueuedConnection);
}

bool Server::listen(quint16 port)
{
    return tcpServer.listen(QHostAddress::Any, port);
}

QString Server::getIp()
{
    QTcpSocket tcpSocket;

    tcpSocket.connectToHost("www.stu.edu.cn", 80);
    tcpSocket.waitForConnected();
    if(tcpSocket.state() == QTcpSocket::ConnectedState)
        return tcpSocket.localAddress().toString();

    return QString("127.0.0.1");
}

quint16 Server::getPort()
{
    return tcpServer.serverPort();
}

void Server::newConnection()
{
    extern DataPool g_dataPool;
    QTcpSocket *tcpSocket;

    tcpSocket = tcpServer.nextPendingConnection();
    if(tcpSocket == NULL)
        return;
    g_dataPool.setNewConnection(tcpSocket);
}
