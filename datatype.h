#ifndef DATATYPE_H
#define DATATYPE_H

#include <QString>

class UserProfile
{
public:
    quint32 account;
    QString name;
    QString sex;
    QString area;
    QString pwd;
};

class FriendMessage
{
public:
    quint32 account;
    bool    isOnLine;
    QString displayName;
    QString nickName;
    QString sex;
    QString area;
};

class AddrInfo
{
public:
    QString ipAddr;
    quint16 port;
};

#endif // DATATYPE_H
