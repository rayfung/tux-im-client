#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QFile>
#include <QTimer>
#include <QTime>
#include <QAudioInput>
#include <QAudioOutput>
#include <QAudioFormat>
#include <QAudioDeviceInfo>
#include "datatype.h"
#include "network/datapool.h"
#include "imapi.h"
#include "inputbox.h"

namespace Ui {
class ChatWindow;
}

class ChatWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ChatWindow(UserProfile me, FriendProfile friendInfo, QWidget *parent = 0);
    ~ChatWindow();
    void setUserInfo(UserProfile info);
    void setFriendInfo(FriendProfile info);

private:
    bool establishConnection(enum Connection::ConnectionType type, QString path = QString());
    void showButtons(bool chooseFile, bool cancel, bool accept, bool reject);
    void showAudioButtons(bool start, bool cancel, bool accept, bool reject);
    void setupAudioIO();
    QString getFileSizeAsString(qint64 size);
    void closeEvent(QCloseEvent *e);
    void showEvent(QShowEvent *e);

private slots:
    void newMessage(quint32 peerUID, QString msg);
    void fileRequest(quint32 peerUID, QString fileName, quint64 fileSize);
    void fileRequestResult(quint32 peerUID, bool accepted);
    void connectionAborted(quint32 peerUID, Connection::ConnectionType type);
    void fileDataReady(quint32 peerUID, QByteArray data);
    void fileReceived(quint32 peerUID);
    void fileSendData();
    void audioRequestResult(quint32 peerUID, bool accepted);
    void audioRequest(quint32 peerUID);
    void audioDataReady(quint32 peerUID, QByteArray data);
    void inDeviceReadyRead();

    void on_pushButtonChooseFile_clicked();
    void on_toolButtonClear_clicked();
    void on_fontComboBox_currentFontChanged(const QFont &f);
    void on_pushButtonClose_clicked();
    void on_comboBoxFontSize_currentIndexChanged(const QString &size);
    void on_pushButtonSend_clicked();
    void on_toolButtonBold_toggled(bool checked);
    void on_toolButtonItalic_toggled(bool checked);
    void on_pushButtonFileCancel_clicked();
    void on_pushButtonRejectFile_clicked();
    void on_pushButtonAcceptFile_clicked();
    void on_pushButtonAudioStart_clicked();
    void on_pushButtonAudioReject_clicked();
    void on_pushButtonAudioAccept_clicked();
    void on_pushButtonAudioEnd_clicked();

private:
    Ui::ChatWindow *ui;
    InputBox *inputBox;
    static const int oneKB = 1024;
    static const int oneMB = 1024 * 1024;
    static  const int oneGB = 1024 * 1024 * 1024;
    UserProfile me;
    FriendProfile friendInfo;
    IMAPI api;
    QString fileName;
    QFile receivedFile;
    QFile sendingFile;
    QTimer fileSendingTimer;
    enum
    {
        StateFileReceived,
        StateFileSent,
        StateFileAborted,
        StateFileRejected
    }fileState;
    quint64 receiveFileSize;
    QAudioInput *audioInput;
    QAudioOutput *audioOutput;
    QIODevice *inDevice;
    QIODevice *outDevice;
    QTime audioTime;
};

#endif // CHATWINDOW_H
