#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QObject>

class Server : public QObject
{
    Q_OBJECT
public:
    Server(QObject *parent = 0);

    bool    listen(quint16 port);
    QString getIp();
    quint16 getPort();

private slots:
    void newConnection();

private:
    QTcpServer  tcpServer;
};

#endif // SERVER_H
