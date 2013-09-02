#ifndef IMAPI_H
#define IMAPI_H

#include "netsocket.h"
#include "datatype.h"

#include <QTcpSocket>
#include <QString>
#include <QByteArray>
#include <QVector>

class IMAPI
{
public:
    IMAPI();
    ~IMAPI();

    bool registration(QTcpSocket *tcpSocket, QString pwd, QString name,
                      QString sex, QString area, quint32 &account);
    bool login(QTcpSocket *tcpSocket, quint32 account, QString pwd,
               QString ip, quint16 port, UserMessage &userMessage);
    bool getUserList(QTcpSocket *tcpSocket, QVector<FriendMessage> &friendMessageList);
    bool getUserProfile(QTcpSocket *tcpSocket, quint32 account,
                        QVector<FriendMessage> &friendList);
    bool deleteFriend(QTcpSocket *tcpSocket, quint32 account);
    bool addFriend(QTcpSocket *tcpSocket, quint32 account, QString displayName);
    bool getIpAndPort(QTcpSocket *tcpSocket, quint32 account, AddrMessage &addrMessage);
    bool modifyFriendDisplayName(QTcpSocket *tcpSocket, quint32 account, QString newName);
    bool modifyPwd(QTcpSocket *tcpSocket, QString pwd);
    bool modifyUserMessage(QTcpSocket *tcpSocket, QString name, QString sex, QString area);

    bool readyToSendMessage(QTcpSocket *tcpSocket, quint32 account);
    bool SendMessage(QTcpSocket *tcpSocket, QString msg);
    bool readyToSendFile(QTcpSocket *tcpSocket, quint32 account);
    bool requestToAcceptFile(QTcpSocket *tcpSocket, QString fileName, quint64 size);
    void SendFile(QTcpSocket *tcpSocket, QByteArray bytes);
    bool stopSendFile(QTcpSocket *tcpSocket);
    bool readyToVoiceChat(QTcpSocket *tcpSocket, quint32 account);
    bool requestToVoiceChat(QTcpSocket *tcpSocket);
    void voiceChat(QTcpSocket *tcpSocket, QByteArray bytes);
    bool stopVoiceChat(QTcpSocket *tcpSocket);

private:
    NetSocket *netSocket;
};

#endif // IMAPI_H
