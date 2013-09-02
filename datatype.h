#ifndef DATATYPE_H
#define DATATYPE_H

#include <QString>

typedef struct {
    quint32 account;
    QString name;
    QString  sex;
    QString area;
    QString pwd;
} UserMessage;

typedef struct {
    quint32 account;
    bool    isOnLine;
    QString displayName;
    QString nickName;
    QString sex;
    QString area;
} FriendMessage;

typedef struct {
    QString ipAddr;
    quint16 port;
}  AddrMessage;

#endif // DATATYPE_H
