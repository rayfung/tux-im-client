#include "chatwindow.h"
#include "ui_chatwindow.h"
#include "utils.h"
#include <QMessageBox>
#include <QDateTime>

extern DataPool g_dataPool;

ChatWindow::ChatWindow(UserProfile me, FriendProfile friendInfo, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ChatWindow)
{
    ui->setupUi(this);
    showButtons(true, false, false, false);

    setUserInfo(me);
    setFriendInfo(friendInfo);

    inputBox = new InputBox();
    connect(inputBox, SIGNAL(returnPressed()), this, SLOT(on_pushButtonSend_clicked()));
    inputBox->setMaximumHeight(80);
    ui->verticalLayoutInput->addWidget(inputBox);

    QString num;
    for(int i = 12; i <= 24; i+=2)
    {
        num.setNum(i);
        ui->comboBoxFontSize->addItem(num);
    }

    connect(&g_dataPool, SIGNAL(newMessage(quint32,QString)),
            this, SLOT(newMessage(quint32,QString)));
    connect(&g_dataPool, SIGNAL(fileRequest(quint32,QString,quint64)),
            this, SLOT(fileRequest(quint32,QString,quint64)));
}

ChatWindow::~ChatWindow()
{
    delete ui;
}

void ChatWindow::setUserInfo(UserProfile info)
{
    me = info;
}

void ChatWindow::setFriendInfo(FriendProfile info)
{
    friendInfo = info;
    if(friendInfo.displayName.isEmpty())
        this->setWindowTitle(friendInfo.nickName);
    else
        this->setWindowTitle(friendInfo.displayName + " (" + friendInfo.nickName + ")");
}

void ChatWindow::showButtons(bool chooseFile, bool cancel, bool accept, bool reject)
{
    ui->pushButtonChooseFile->setVisible(chooseFile);
    ui->pushButtonFileCancel->setVisible(cancel);
    ui->pushButtonAcceptFile->setVisible(accept);
    ui->pushButtonRejectFile->setVisible(reject);
}

void ChatWindow::fileRequest(quint32 peerUID, QString fileName, quint64 fileSize)
{
    if(peerUID != friendInfo.account)
        return;

    ui->labelFileName->setText(fileName);
    ui->labelFileSize->setText(getFileSizeAsString(fileSize));
    ui->labelFileState->setText("对方向你发送文件");
    showButtons(false, false, true, true);
}

QString ChatWindow::getFileSizeAsString(qint64 size)
{
    double d;
    QString unit;

    if(size < 1024)
    {
        d = size;
        unit = "B";
    }
    else if(size < 1024L * 1024L)
    {
        d = (double)size / 1024.0;
        unit = "KB";
    }
    else if(size < 1024L * 1024L * 1024L)
    {
        d = (double)size / 1024.0 / 1024.0;
        unit = "MB";
    }
    else
    {
        d = (double)size / 1024.0 / 1024.0 / 1024.0;
        unit = "GB";
    }
    return QString("%1 %2").arg(d, 0, 'f', 2).arg(unit);
}

void ChatWindow::on_pushButtonChooseFile_clicked()
{
    QString path;

    path = QFileDialog::getOpenFileName(this, "选择文件");
    if(path.isNull())
        return;
    QFileInfo info(path);

    ui->labelFileName->setText(info.fileName());
    ui->labelFileSize->setText(getFileSizeAsString(info.size()));
    ui->labelFileState->setText("等待对方接收");
    if(!establishConnection(Connection::file_connection, path))
    {
        QMessageBox::warning(this, "提示", "对方可能不在线，发送文件失败");
        ui->labelFileState->setText("无传输");
        return;
    }
    showButtons(false, true, false, false);
}

void ChatWindow::on_toolButtonClear_clicked()
{
    ui->textEditChatMsg->clear();
}

void ChatWindow::on_fontComboBox_currentFontChanged(const QFont &f)
{
    inputBox->setFontFamily(f.family());
}

void ChatWindow::on_pushButtonClose_clicked()
{
    this->close();
}

void ChatWindow::on_comboBoxFontSize_currentIndexChanged(const QString &size)
{
    inputBox->setFontPointSize(size.toDouble());
}

bool ChatWindow::establishConnection(enum Connection::ConnectionType type, QString path)
{
    AddrInfo addr;
    QTcpSocket *socket;

    if(g_dataPool.isConnected(friendInfo.account, type))
        return true;
    if(!api.getIpAndPort(Utils::getInstance()->getTcpSocket(), friendInfo.account, addr))
        return false;
    socket = new QTcpSocket();
    socket->connectToHost(addr.ipAddr, addr.port);
    if(!socket->waitForConnected(5000))
        return false;
    switch(type)
    {
    case Connection::message_connection:
        g_dataPool.setMessageConnection(socket, me.account, friendInfo.account);
        break;

    case Connection::file_connection:
        g_dataPool.setFileConnection(socket, me.account, path, friendInfo.account);
        break;

    case Connection::audio_connection:
        g_dataPool.setAudioConnection(socket, me.account, friendInfo.account);
        break;

    default:
        break;
    }
    return true;
}

void ChatWindow::on_pushButtonSend_clicked()
{
    QString header;
    QString text = inputBox->toHtml();

    header = QString("<span style='font-size:11pt; color:#0000ff;'>%1</span>");
    header = header.arg(Qt::escape(me.name + " "
                        + QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss")));
    if(!inputBox->toPlainText().isEmpty())
    {
        ui->textEditChatMsg->append(header);
        ui->textEditChatMsg->append(text);
        inputBox->clear();
        if(!establishConnection(Connection::message_connection))
        {
            QMessageBox::warning(this, "提示", "对方可能不在线，无法发送消息");
            return;
        }
        g_dataPool.sendMessage(friendInfo.account, text);
    }
}

void ChatWindow::newMessage(quint32 peerUID, QString msg)
{
    if(peerUID != friendInfo.account)
        return;

    QString header;
    QString name;

    if(friendInfo.displayName.isEmpty())
        name = friendInfo.nickName;
    else
        name = friendInfo.displayName;

    header = QString("<span style='font-size:11pt; color:#00aa00;'>%1</span>");
    header = header.arg(Qt::escape(name + " "
                        + QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss")));
    ui->textEditChatMsg->append(header);
    ui->textEditChatMsg->append(msg);
}

void ChatWindow::on_toolButtonBold_toggled(bool checked)
{
    if(checked)
        inputBox->setFontWeight(QFont::Bold);
    else
        inputBox->setFontWeight(QFont::Normal);
}

void ChatWindow::on_toolButtonItalic_toggled(bool checked)
{
    inputBox->setFontItalic(checked);
}

void ChatWindow::closeEvent(QCloseEvent *e)
{
    inputBox->clear();
    e->accept();
}

void ChatWindow::showEvent(QShowEvent *e)
{
    inputBox->setFocus();
    e->accept();
}
