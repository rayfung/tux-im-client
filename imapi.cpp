#include "imapi.h"
#include <QDataStream>
#include <QIODevice>

IMAPI::IMAPI()
{
    netSocket = new NetSocket();
}

IMAPI::~IMAPI()
{
    delete netSocket;
}

bool IMAPI::registration(QTcpSocket *tcpSocket, QString pwd, QString name, QString sex,
                         QString area, quint32 &account)
{
    QByteArray   bytes;
    QByteArray   inBytes;
    QDataStream  out(&bytes, QIODevice::WriteOnly);
    QDataStream  in(&inBytes, QIODevice::ReadOnly);

    in.setVersion(QDataStream::Qt_4_6);
    out.setVersion(QDataStream::Qt_4_6);
    out << (quint8)0x01;
    out << pwd << name << sex << area;
    netSocket->sendData(tcpSocket, bytes);
    if(!netSocket->readData(tcpSocket, inBytes))
        return false;
    in >> account;
    if(account == 0)
        return false;

    return true;
}

bool IMAPI::login(QTcpSocket *tcpSocket, quint32 account, QString pwd,
                  QString ip, quint16 port, UserProfile &userProfile)
{
    bool        flag;
    QByteArray  bytes;
    QByteArray  inBytes;
    QDataStream out(&bytes, QIODevice::WriteOnly);
    QDataStream in(&inBytes, QIODevice::ReadOnly);

    in.setVersion(QDataStream::Qt_4_6);
    out.setVersion(QDataStream::Qt_4_6);

    out << (quint8)0x02;
    out << account << pwd << ip << port;
    netSocket->sendData(tcpSocket, bytes);
    if(!netSocket->readData(tcpSocket, inBytes))
        return false;

    in >> flag;
    if(!flag)
        return false;
    in >> (userProfile.name) >> (userProfile.sex) >> (userProfile.area);

    return true;
}

bool IMAPI::getUserList(QTcpSocket *tcpSocket, QVector<FriendProfile> &friendList)
{
    quint32         count;
    FriendProfile   friendProfile;
    QByteArray      bytes;
    QByteArray      inBytes;
    QDataStream     out(&bytes, QIODevice::WriteOnly);
    QDataStream     in(&inBytes, QIODevice::ReadOnly);

    in.setVersion(QDataStream::Qt_4_6);
    out.setVersion(QDataStream::Qt_4_6);
    out << (quint8)0x03;
    netSocket->sendData(tcpSocket, bytes);
    if(!netSocket->readData(tcpSocket, inBytes))
        return false;
    in >> count;
    for(quint32 i = 0; i < count; i++)
    {
        in >> (friendProfile.account) >> (friendProfile.isOnLine) >> (friendProfile.nickName)
           >> (friendProfile.displayName) >> (friendProfile.sex) >> (friendProfile.area);
        friendList.push_back(friendProfile);
    }

    return true;
}

bool IMAPI::getUserProfile(QTcpSocket *tcpSocket, quint32 account,
                             QVector<FriendProfile> &friendList)
{
    quint32         count;
    FriendProfile   friendMessage;
    QByteArray      bytes;
    QByteArray      inBytes;
    QDataStream     in(&inBytes, QIODevice::ReadOnly);
    QDataStream     out(&bytes, QIODevice::WriteOnly);

    in.setVersion(QDataStream::Qt_4_6);
    out.setVersion(QDataStream::Qt_4_6);
    out << (quint8)0x04;
    out << account;
    netSocket->sendData(tcpSocket, bytes);
    if(!netSocket->readData(tcpSocket, inBytes))
        return false;
    in >> count;
    if(count == 1)
    {
        in >> (friendMessage.account) >> (friendMessage.isOnLine) >> (friendMessage.nickName)
           >> (friendMessage.displayName) >> (friendMessage.sex) >> (friendMessage.area);
        friendList.push_back(friendMessage);
    }

    return true;
}

bool IMAPI::deleteFriend(QTcpSocket *tcpSocket, quint32 account)
{
    bool        flag;
    QByteArray  inBytes;
    QByteArray  outBytes;
    QDataStream in(&inBytes, QIODevice::ReadOnly);
    QDataStream out(&outBytes, QIODevice::WriteOnly);

    in.setVersion(QDataStream::Qt_4_6);
    out.setVersion(QDataStream::Qt_4_6);
    out << (quint8)0x05;
    out << account;
    netSocket->sendData(tcpSocket, outBytes);
    if(!netSocket->readData(tcpSocket, inBytes))
        return false;
    in >> flag;
    if(!flag)
        return false;

    return true;
}

bool IMAPI::addFriend(QTcpSocket *tcpSocket, quint32 account, QString displayName)
{
    bool        flag;
    QByteArray  bytes;
    QByteArray  inBytes;
    QDataStream in(&inBytes, QIODevice::ReadOnly);
    QDataStream out(&bytes, QIODevice::WriteOnly);

    in.setVersion(QDataStream::Qt_4_6);
    out.setVersion(QDataStream::Qt_4_6);
    out << (quint8)0x06;
    out << account;
    out << displayName;
    netSocket->sendData(tcpSocket, bytes);
    if(!netSocket->readData(tcpSocket, inBytes))
        return false;
    in >> flag;
    if(!flag)
        return false;

    return true;
}

bool IMAPI::getIpAndPort(QTcpSocket *tcpSocket, quint32 account, AddrInfo &addr)
{
    bool        flag;
    QByteArray  outBytes;
    QByteArray  bytes;
    QDataStream in(&bytes, QIODevice::ReadOnly);
    QDataStream out(&outBytes, QIODevice::WriteOnly);

    in.setVersion(QDataStream::Qt_4_6);
    out.setVersion(QDataStream::Qt_4_6);
    out << (quint8)0x07;
    out << account;
    netSocket->sendData(tcpSocket, outBytes);
    if(!netSocket->readData(tcpSocket, bytes))
        return false;
    in >> flag;
    if(!flag)
        return false;
    in >> (addr.ipAddr) >> (addr.port);

    return true;
}

bool IMAPI::modifyFriendDisplayName(QTcpSocket *tcpSocket, quint32 account, QString newName)
{
    bool        flag;
    QByteArray  inBytes;
    QByteArray  outBytes;
    QDataStream in(&inBytes, QIODevice::ReadOnly);
    QDataStream out(&outBytes, QIODevice::WriteOnly);

    in.setVersion(QDataStream::Qt_4_6);
    out.setVersion(QDataStream::Qt_4_6);
    out << (quint8)0xE0;
    out << account;
    out << newName;
    netSocket->sendData(tcpSocket, outBytes);
    if(!netSocket->readData(tcpSocket, inBytes))
        return false;
    in >> flag;
    if(!flag)
        return false;

    return true;
}

bool IMAPI::modifyPwd(QTcpSocket *tcpSocket, QString pwd)
{
    bool        flag;
    QByteArray  inBytes;
    QByteArray  outBytes;
    QDataStream in(&inBytes, QIODevice::ReadOnly);
    QDataStream out(&outBytes, QIODevice::WriteOnly);

    in.setVersion(QDataStream::Qt_4_6);
    out.setVersion(QDataStream::Qt_4_6);
    out << (quint8)0xE1;
    out << pwd;
    netSocket->sendData(tcpSocket, outBytes);
    if(!netSocket->readData(tcpSocket, inBytes))
        return false;
    in >> flag;
    if(!flag)
        return false;

    return true;
}

bool IMAPI::modifyUserMessage(QTcpSocket *tcpSocket, QString name,
                              QString sex, QString area)
{
    bool        flag;
    QByteArray  inBytes;
    QByteArray  outBytes;
    QDataStream in(&inBytes, QIODevice::ReadOnly);
    QDataStream out(&outBytes, QIODevice::WriteOnly);

    in.setVersion(QDataStream::Qt_4_6);
    out.setVersion(QDataStream::Qt_4_6);
    out << (quint8)0xE2;
    out << name;
    out << sex;
    out << area;
    netSocket->sendData(tcpSocket, outBytes);
    if(!netSocket->readData(tcpSocket, inBytes))
        return false;
    in >> flag;
    if(!flag)
        return false;

    return true;
}

bool IMAPI::readyToSendMessage(QTcpSocket *tcpSocket, quint32 account)
{
    QByteArray   bytes;
    QDataStream  out(&bytes, QIODevice::WriteOnly);

    out.setVersion(QDataStream::Qt_4_6);
    out << (quint8)0x08;
    out << account;
    netSocket->sendData(tcpSocket, bytes);
    if(!netSocket->readData(tcpSocket))
        return false;

    return true;
}

bool IMAPI::SendMessage(QTcpSocket *tcpSocket, QString msg)
{
    QByteArray  bytes;
    QDataStream out(&bytes, QIODevice::WriteOnly);

    out.setVersion(QDataStream::Qt_4_6);
    out << (quint8)0x09;
    out << msg;
    netSocket->sendData(tcpSocket, bytes);
    if(!netSocket->readData(tcpSocket))
        return false;

    return true;
}

bool IMAPI::readyToSendFile(QTcpSocket *tcpSocket, quint32 account)
{
    QByteArray  bytes;
    QDataStream out(&bytes, QIODevice::WriteOnly);

    out.setVersion(QDataStream::Qt_4_6);
    out << (quint8)0x0A;
    out << account;
    netSocket->sendData(tcpSocket, bytes);
    if(!netSocket->readData(tcpSocket))
        return false;

    return true;
}

bool IMAPI::requestToAcceptFile(QTcpSocket *tcpSocket, QString fileName, quint64 size)
{
    bool        flag;
    QByteArray  outBytes;
    QByteArray  inBytes;
    QDataStream in(&inBytes, QIODevice::ReadOnly);
    QDataStream out(&outBytes, QIODevice::WriteOnly);

    in.setVersion(QDataStream::Qt_4_6);
    out.setVersion(QDataStream::Qt_4_6);
    out << (quint8)0x0B;
    out << fileName << size;
    netSocket->sendData(tcpSocket, outBytes);
    if(!netSocket->readData(tcpSocket, inBytes))
        return false;
    in >> flag;
    if(!flag)
        return false;

    return true;
}

void IMAPI::SendFile(QTcpSocket *tcpSocket, QByteArray bytes)
{
    QByteArray  outBytes;
    QDataStream out(&outBytes, QIODevice::WriteOnly);

    out.setVersion(QDataStream::Qt_4_6);
    out << (quint8)0x0C;
    outBytes += bytes;
    netSocket->sendData(tcpSocket, outBytes);
}

bool IMAPI::stopSendFile(QTcpSocket *tcpSocket)
{
    QByteArray  bytes;
    QDataStream out(&bytes, QIODevice::WriteOnly);

    out.setVersion(QDataStream::Qt_4_6);
    out << (quint8)0x0D;
    netSocket->sendData(tcpSocket, bytes);
    if(!netSocket->readData(tcpSocket))
        return false;

    return true;
}

bool IMAPI::readyToVoiceChat(QTcpSocket *tcpSocket, quint32 account)
{
    QByteArray  bytes;
    QDataStream out(&bytes, QIODevice::WriteOnly);

    out.setVersion(QDataStream::Qt_4_6);
    out << (quint8)0x0E;
    out << account;
    netSocket->sendData(tcpSocket, bytes);
    if(!netSocket->readData(tcpSocket))
        return false;

    return true;
}

bool IMAPI::requestToVoiceChat(QTcpSocket *tcpSocket)
{
    bool        flag;
    QByteArray  outBytes;
    QByteArray  inBytes;
    QDataStream in(&inBytes, QIODevice::ReadOnly);
    QDataStream out(&outBytes, QIODevice::WriteOnly);

    in.setVersion(QDataStream::Qt_4_6);
    out.setVersion(QDataStream::Qt_4_6);
    out << (quint8)0x0F;
    netSocket->sendData(tcpSocket, outBytes);
    if(!netSocket->readData(tcpSocket))
        return false;
    in >> flag;
    if(!flag)
        return false;

    return true;
}

void IMAPI::voiceChat(QTcpSocket *tcpSocket, QByteArray bytes)
{
    QByteArray  outBytes;
    QDataStream out(&outBytes, QIODevice::WriteOnly);

    out.setVersion(QDataStream::Qt_4_6);
    out << (quint8)0x10;
    outBytes += bytes;
    netSocket->sendData(tcpSocket, outBytes);
}

bool IMAPI::stopVoiceChat(QTcpSocket *tcpSocket)
{
    QByteArray  bytes;
    QDataStream out(&bytes, QIODevice::WriteOnly);

    out.setVersion(QDataStream::Qt_4_6);
    out << (quint8)0x11;
    netSocket->sendData(tcpSocket, bytes);
    if(!netSocket->readData(tcpSocket))
        return false;

    return true;
}
