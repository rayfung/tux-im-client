#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
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
    explicit ChatWindow(UserMessage me, FriendMessage friendInfo, QWidget *parent = 0);
    ~ChatWindow();
    void setUserInfo(UserMessage info);
    void setFriendInfo(FriendMessage info);

private:
    bool establishConnection(enum Connection::ConnectionType type, QString path = QString());
    void showButtons(bool chooseFile, bool cancel, bool accept, bool reject);
    QString getFileSizeAsString(qint64 size);
    void closeEvent(QCloseEvent *e);
    void showEvent(QShowEvent *e);

private slots:
    void newMessage(quint32 peerUID, QString msg);
    void fileRequest(quint32 peerUID, QString fileName, quint64 fileSize);
    void on_pushButtonChooseFile_clicked();
    void on_toolButtonClear_clicked();
    void on_fontComboBox_currentFontChanged(const QFont &f);
    void on_pushButtonClose_clicked();
    void on_comboBoxFontSize_currentIndexChanged(const QString &size);
    void on_pushButtonSend_clicked();
    void on_toolButtonBold_toggled(bool checked);
    void on_toolButtonItalic_toggled(bool checked);

private:
    Ui::ChatWindow *ui;
    InputBox *inputBox;
    static const int oneKB = 1024;
    static const int oneMB = 1024 * 1024;
    static  const int oneGB = 1024 * 1024 * 1024;
    UserMessage me;
    FriendMessage friendInfo;
    IMAPI api;
};

#endif // CHATWINDOW_H
