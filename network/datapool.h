#ifndef DATAPOOL_H
#define DATAPOOL_H

#include <QString>
#include <QObject>
#include <QByteArray>
#include <QTcpSocket>
#include <QMap>

class Connection
{
public:
    Connection();
    quint32     peerUID; //对方的账号
    QTcpSocket  *tcpSocket;
    quint32 len;
    enum
    {
        state_recv_len,
        state_recv_data,
        state_wait,
        state_loop,
        state_loop_data,
        state_close,
        state_done
    } state;
    enum ConnectionType
    {
        message_connection,
        file_connection,
        audio_connection,
        unknown_connection
    } connectionType;
};

class DataPool : public QObject
{
    Q_OBJECT
public:
    explicit DataPool(QObject *parent = 0);

    void setNewConnection(QTcpSocket *tcpSocket);
    void setMessageConnection(QTcpSocket *tcpSocket, quint32 uid, quint32 peerUID);
    void setFileConnection(QTcpSocket *tcpSocket, quint32 uid, QString path, quint32 peerUID);
    void setAudioConnection(QTcpSocket *tcpSocket, quint32 uid, quint32 peerUID);

    bool sendMessage(quint32 peerUID, QString msg);
    void finishSendMessage(quint32 peerUID);
    bool sendFileData(quint32 peerUID, QByteArray bytes);
    void finishSendFile(quint32 peerUID);
    void abortSendFile(quint32 peerUID);
    bool sendAudioData(quint32 peerUID, QByteArray bytes);
    void finishSendAudio(quint32 peerUID);

    bool sendFileRequestResult(quint32 peerUID, bool accepted);
    bool sendAudioRequestResult(quint32 peerUID, bool accepted);

private:
    void sendData(QTcpSocket *tcpSocket, QByteArray bytes);
    Connection *findConnectionByUID(quint32 peerUID,
                                        enum Connection::ConnectionType type);
    bool sendRequestResult(quint32 peerUID, bool accepted,
                           enum Connection::ConnectionType type);

private slots:
    void readyRead();
    void cleanup();

private:
    QMap<QTcpSocket*, Connection>   accountMap;

signals:
    void newMessage(quint32, QString); //新文本消息到来

    void fileRequest(quint32, QString, quint64); //文件传输请求
    void fileRequestResult(quint32, bool); //文件传输请求结果
    void fileDataReady(quint32, QByteArray); //文件数据到达
    void fileReceived(quint32); //文件已经接收完毕

    void audioRequest(quint32); //请求语音通话
    void audioRequestResult(quint32, bool); //语音通话请求结果
    void audioDataReady(quint32, QByteArray); //语音数据到达

    void connectionAborted(quint32); //结束文本通信、终止文件传输、结束语音通信
};

#endif // DATAPOOL_H
