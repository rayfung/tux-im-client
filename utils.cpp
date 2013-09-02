#include "utils.h"
#include "config.h"

Utils *Utils::utils = NULL;

Utils::Utils()
{
    tcpSocket = new QTcpSocket;
}

Utils *Utils::getInstance()
{
    Config  config;

    if(utils == NULL)
        utils = new Utils;
    if(utils->tcpSocket->state() == QAbstractSocket::UnconnectedState)
    {
        utils->tcpSocket->connectToHost(config.ipAddress, config.port);
    }

    return utils;
}

QTcpSocket *Utils::getTcpSocket()
{
    return tcpSocket;
}
