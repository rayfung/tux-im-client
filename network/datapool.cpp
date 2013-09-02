#include "datapool.h"
#include <QFileInfo>

Connection::Connection()
{
    peerUID = 0;
    tcpSocket = NULL;
    state = state_recv_len;
    connectionType = unknown_connection;
}

DataPool::DataPool(QObject *parent) : QObject(parent)
{
}

void DataPool::readyRead()
{
    QTcpSocket *socket;

    socket = qobject_cast<QTcpSocket *>(sender());
    if(socket == NULL)
        return;
    if(!accountMap.contains(socket))
        return;

    Connection &conn = accountMap[socket];
    if(conn.tcpSocket != socket)
    {
        qWarning() << "DataPool::readyRead() tcpSocket not the same";
        return;
    }

    while(1)
    {
        switch(conn.state)
        {
        case Connection::state_recv_len:
            if(conn.tcpSocket->bytesAvailable() < sizeof(quint32))
                return;
            do
            {
                QDataStream in(socket);
                in.setVersion(QDataStream::Qt_4_6);
                in >> conn.len;
            }while(0);
            conn.state = Connection::state_recv_data;
            break;

        case Connection::state_recv_data:
            if(conn.tcpSocket->bytesAvailable() < conn.len)
                return;
            conn.state = Connection::state_loop;
            if(conn.connectionType == Connection::unknown_connection)
            {
                quint8 cmd;
                QString fileName;
                quint64 fileSize;
                QDataStream in(socket);
                in.setVersion(QDataStream::Qt_4_6);
                in >> cmd;
                switch(cmd)
                {
                case 0x08:
                    conn.connectionType = Connection::message_connection;
                    in >> conn.peerUID;
                    break;

                case 0x0A:
                    in >> conn.peerUID >> fileName >> fileSize;
                    conn.connectionType = Connection::file_connection;
                    conn.state = Connection::state_wait;
                    emit fileRequest(conn.peerUID, fileName, fileSize);
                    return;

                case 0x0D:
                    in >> conn.peerUID;
                    conn.connectionType = Connection::audio_connection;
                    conn.state = Connection::state_wait;
                    emit audioRequest(conn.peerUID);
                    return;

                default:
                    qDebug() << "#1:" << cmd;
                    conn.state = Connection::state_close;
                    conn.tcpSocket->close();
                    return;
                }
            }
            else
            {
                bool accepted;
                QDataStream in(socket);
                in.setVersion(QDataStream::Qt_4_6);
                in >> accepted;
                if(conn.connectionType == Connection::file_connection)
                    emit fileRequestResult(conn.peerUID, accepted);
                else if(conn.connectionType == Connection::audio_connection)
                    emit audioRequestResult(conn.peerUID, accepted);
            }
            break;

        case Connection::state_wait:
            qDebug() << "#2";
            conn.state = Connection::state_close;
            conn.tcpSocket->close();
            return;

        case Connection::state_loop:
            if(conn.tcpSocket->bytesAvailable() < sizeof(quint32))
                return;
            do
            {
                QDataStream in(socket);
                in.setVersion(QDataStream::Qt_4_6);
                in >> conn.len;
            }while(0);
            conn.state = Connection::state_loop_data;
            break;

        case Connection::state_loop_data:
            if(conn.tcpSocket->bytesAvailable() < conn.len)
                return;
            do
            {
                quint8 cmd;
                QString msg;
                QByteArray data;
                QDataStream in(socket);
                in.setVersion(QDataStream::Qt_4_6);
                in >> cmd;
                conn.state = Connection::state_loop;
                switch(conn.connectionType)
                {
                case Connection::message_connection:
                    in >> msg;
                    emit newMessage(conn.peerUID, msg);
                    break;

                case Connection::file_connection:
                    if(cmd == 0x0B)
                    {
                        in >> data;
                        emit fileDataReady(conn.peerUID, data);
                    }
                    else if(cmd == 0x0C)
                    {
                        conn.state = Connection::state_done;
                        conn.tcpSocket->close();
                        emit fileReceived(conn.peerUID);
                        return;
                    }
                    break;

                case Connection::audio_connection:
                    in >> data;
                    emit audioDataReady(conn.peerUID, data);
                    break;

                default:
                    return;
                }
            }while(0);
            break;

        case Connection::state_close:
        case Connection::state_done:
            return;
        }
    }
}

bool DataPool::sendRequestResult(quint32 peerUID, bool accepted,
                                 Connection::ConnectionType type)
{
    Connection *conn;

    conn = findConnectionByUID(peerUID, type);
    if(conn == NULL)
        return false;
    if(conn->state != Connection::state_wait)
        return false;
    if(type != Connection::file_connection && type != Connection::audio_connection)
        return false;

    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);

    out.setVersion(QDataStream::Qt_4_6);
    out << accepted;
    sendData(conn->tcpSocket, data);
    conn->state = Connection::state_loop;
    return true;
}

bool DataPool::sendFileRequestResult(quint32 peerUID, bool accepted)
{
    return sendRequestResult(peerUID, accepted, Connection::file_connection);
}

bool DataPool::sendAudioRequestResult(quint32 peerUID, bool accepted)
{
    return sendRequestResult(peerUID, accepted, Connection::audio_connection);
}

void DataPool::cleanup()
{
    QTcpSocket *socket;

    socket = qobject_cast<QTcpSocket *>(sender());
    if(socket)
    {
        qDebug() << accountMap[socket].state;
        if(accountMap[socket].state != Connection::state_done)
            emit connectionAborted(accountMap[socket].peerUID);
        accountMap.remove(socket);
    }
}

Connection *DataPool::findConnectionByUID(quint32 peerUID,
                                             enum Connection::ConnectionType type)
{
    QMap<QTcpSocket*, Connection>::iterator iter;

    for(iter = accountMap.begin(); iter != accountMap.end(); ++iter)
    {
        if(iter->peerUID == peerUID && iter->connectionType == type)
            return &(*iter);
    }

    return NULL;
}

void DataPool::sendData(QTcpSocket *tcpSocket, QByteArray bytes)
{
    QByteArray  outBytes;
    QDataStream out(&outBytes, QIODevice::WriteOnly);

    out.setVersion(QDataStream::Qt_4_6);
    out << (quint32)bytes.size();
    outBytes += bytes;
    tcpSocket->write(outBytes);
}

void DataPool::setNewConnection(QTcpSocket *tcpSocket)
{
    Connection connection;

    connection.tcpSocket = tcpSocket;
    connection.state     = Connection::state_recv_len;
    connection.connectionType = Connection::unknown_connection;
    accountMap[tcpSocket] = connection;
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(cleanup()));
}

void DataPool::setMessageConnection(QTcpSocket *tcpSocket, quint32 uid, quint32 peerUID)
{
    QByteArray  bytes;
    Connection  connection;
    QDataStream out(&bytes, QIODevice::WriteOnly);

    out.setVersion(QDataStream::Qt_4_6);
    out << (quint8)0x08;
    out << uid;
    sendData(tcpSocket, bytes);

    connection.peerUID = peerUID;
    connection.tcpSocket = tcpSocket;
    connection.state = Connection::state_loop;
    connection.connectionType = Connection::message_connection;
    accountMap[tcpSocket] = connection;

    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(cleanup()));
}

void DataPool::setFileConnection(QTcpSocket *tcpSocket, quint32 uid,
                                 QString path, quint32 peerUID)
{
    Connection  connection;
    QByteArray  bytes;
    QDataStream out(&bytes, QIODevice::WriteOnly);
    QFileInfo       fileInfo(path);

    out << (quint8)0x0A;
    out << uid;
    out << fileInfo.fileName();
    out << (quint64)fileInfo.size();
    sendData(tcpSocket, bytes);

    connection.peerUID = peerUID;
    connection.tcpSocket = tcpSocket;
    connection.state = Connection::state_recv_len;
    connection.connectionType = Connection::file_connection;
    accountMap[tcpSocket] = connection;
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(cleanup()));
}

void DataPool::setAudioConnection(QTcpSocket *tcpSocket, quint32 uid, quint32 peerUID)
{
    Connection  connection;
    QByteArray  bytes;
    QDataStream out(&bytes, QIODevice::WriteOnly);

    out.setVersion(QDataStream::Qt_4_6);
    out << (quint8)0x0D;
    out << uid;
    sendData(tcpSocket, bytes);

    connection.peerUID = peerUID;
    connection.tcpSocket = tcpSocket;
    connection.state = Connection::state_recv_len;
    connection.connectionType = Connection::audio_connection;
    accountMap[tcpSocket] = connection;
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(cleanup()));
}

bool DataPool::sendMessage(quint32 peerUID, QString msg)
{
    Connection  *connection;
    QByteArray  bytes;
    QDataStream out(&bytes, QIODevice::WriteOnly);

    out.setVersion(QDataStream::Qt_4_6);
    out << (quint8)0x09;
    out << msg;
    connection = findConnectionByUID(peerUID, Connection::message_connection);
    if(connection == NULL)
        return false;

    sendData(connection->tcpSocket, bytes);
    return true;
}

void DataPool::finishSendMessage(quint32 peerUID)
{
    Connection  *connection;

    connection = findConnectionByUID(peerUID, Connection::message_connection);
    if(connection == NULL)
        return;
    connection->state = Connection::state_close;
    connection->tcpSocket->close();
}

bool DataPool::sendFileData(quint32 peerUID, QByteArray bytes)
{
    QByteArray  outBytes;
    Connection  *connection;
    QDataStream out(&outBytes, QIODevice::WriteOnly);

    out.setVersion(QDataStream::Qt_4_6);
    out << (quint8)0x0B;
    out << bytes;
    connection = findConnectionByUID(peerUID, Connection::file_connection);
    if(connection == NULL)
        return false;

    sendData(connection->tcpSocket, outBytes);
    return true;
}

void DataPool::finishSendFile(quint32 peerUID)
{
    QByteArray  outBytes;
    Connection  *connection;
    QDataStream out(&outBytes, QIODevice::WriteOnly);

    out.setVersion(QDataStream::Qt_4_6);
    out << (quint8)0x0C;
    connection = findConnectionByUID(peerUID, Connection::file_connection);
    if(connection == NULL)
        return;
    sendData(connection->tcpSocket, outBytes);
}

void DataPool::abortSendFile(quint32 peerUID)
{
    Connection  *connection;

    connection = findConnectionByUID(peerUID, Connection::file_connection);
    if(connection == NULL)
        return;
    connection->state = Connection::state_close;
    connection->tcpSocket->close();
}

bool DataPool::sendAudioData(quint32 peerUID, QByteArray bytes)
{
    QByteArray  outBytes;
    Connection  *connection;
    QDataStream out(&outBytes, QIODevice::WriteOnly);

    out.setVersion(QDataStream::Qt_4_6);
    out << (quint8)0x0E;
    out << bytes;
    connection = findConnectionByUID(peerUID, Connection::audio_connection);
    if(connection == NULL)
        return false;

    sendData(connection->tcpSocket, outBytes);
    return true;
}

void DataPool::finishSendAudio(quint32 peerUID)
{
    Connection  *connection;

    connection = findConnectionByUID(peerUID, Connection::audio_connection);
    if(connection == NULL)
        return;
    connection->state = Connection::state_close;
    connection->tcpSocket->close();
}
