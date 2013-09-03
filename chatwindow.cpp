#include "chatwindow.h"
#include "ui_chatwindow.h"
#include "utils.h"
#include <QMessageBox>

extern DataPool g_dataPool;

ChatWindow::ChatWindow(UserMessage me, FriendMessage friendInfo, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ChatWindow),
    me(me),
    friendInfo(friendInfo)
{
    ui->setupUi(this);

    if(friendInfo.displayName.isEmpty())
        this->setWindowTitle(friendInfo.nickName);
    else
        this->setWindowTitle(friendInfo.displayName + " (" + friendInfo.nickName + ")");

    ui->pushButtonFileCancel->setEnabled(false);
    ui->labelFileName->setText("空");
    ui->labelFileSize->setText("0KB");
    ui->labelFileState->setText("无传输");
    QString num;
    for(int i = 12; i <= 24; i+=2)
    {
        num.setNum(i);
        ui->comboBoxFontSize->addItem(num);
    }
    isBold = false;
    isItalic = false;

    connect(&g_dataPool, SIGNAL(newMessage(quint32,QString)),
            this, SLOT(newMessage(quint32,QString)));
}

ChatWindow::~ChatWindow()
{
    delete ui;
}

void ChatWindow::on_pushButtonChooseFile_clicked()
{
    QStringList FilePathList;
    QString FilePath("");
    QFile file;
    QFileDialog fd(this);
    fd.setFileMode(QFileDialog::ExistingFile);
    fd.setViewMode(QFileDialog::List);
    if(fd.exec())
        FilePathList = fd.selectedFiles();


    if(!FilePathList.isEmpty ())
    {


         FilePath = FilePathList.at(0);

         ui->pushButtonFileCancel->setEnabled(true);
         file.setFileName(FilePath);
         file.open(QIODevice::ReadOnly);
         qint64 fileSize = file.size();
         QString sfSize;



         if(fileSize < oneKB)
         {
             sfSize.setNum(fileSize);
             sfSize += "B";

         }
         else if(fileSize < oneMB)
         {
             fileSize /= oneKB;
             sfSize.setNum(fileSize);
             sfSize +="KB";
         }
         else if(fileSize < oneGB )
         {
             fileSize *= 100;
             fileSize /= oneMB;
             int temp = fileSize % 100;
             QString stemp;
             stemp.setNum(temp);
             fileSize /= 100;

             sfSize.setNum(fileSize);
             sfSize += '.';
             sfSize += stemp;
             sfSize +="MB";
         }
         else
         {
             fileSize *= 100;
             fileSize /= oneGB;
             sfSize.setNum(fileSize);
             int temp = fileSize % 100;
             QString stemp;
             stemp.setNum(temp);
             fileSize /= 100;

             sfSize.setNum(fileSize);
             sfSize += '.';
             sfSize += stemp;
             sfSize +="GB";

         }


         ui->labelFileSize->setText(sfSize);
         fileName = "";
         int slen = FilePath.length() - 1;

         while(slen >= 0)
         {
             if(FilePath[slen] == '/')
                 break;
             fileName.prepend(FilePath[slen]);
             --slen;

         }
         ui->labelFileName->setText(fileName);



    }
    else
    {
        ui->pushButtonFileCancel->setEnabled(false);
        ui->labelFileName->setText("空");
        ui->labelFileSize->setText("0KB");
        ui->labelFileState->setText("无传输");

    }

}

void ChatWindow::on_toolButtonClear_clicked()
{
    ui->plainTextEditChatMessage->setPlainText("");
    ui->plainTextEditInput->setPlainText("");

}

void ChatWindow::on_fontComboBox_currentFontChanged(const QFont &f)
{
    ui->plainTextEditInput->setFont(f);
    ui->plainTextEditChatMessage->setFont(f);
    ui->plainTextEditInput->setFocus();
}

void ChatWindow::on_pushButtonClose_clicked()
{
    this->close();
}

void ChatWindow::on_comboBoxFontSize_currentIndexChanged(const QString &arg1)
{
    ui->plainTextEditInput->setFontPointSize(arg1.toDouble());
    ui->plainTextEditChatMessage->setFontPointSize(arg1.toDouble());
    ui->plainTextEditInput->setFocus();

}



void ChatWindow::on_toolButtonBoldToo_clicked()
{
    isBold = !isBold;
    if(isBold)
    {
        ui->plainTextEditInput->setFontWeight(QFont::Bold);
        ui->plainTextEditChatMessage->setFontWeight(QFont::Bold);
    }
    else
    {
        ui->plainTextEditInput->setFontWeight(QFont::Normal);
        ui->plainTextEditChatMessage->setFontWeight(QFont::Normal);
    }

    ui->toolButtonBoldToo->setDown(isBold);
    ui->plainTextEditInput->setFocus();

}

void ChatWindow::on_toolButtonItalic_clicked()
{
    isItalic = !isItalic;
    ui->plainTextEditInput->setFontItalic(isItalic);
    ui->plainTextEditChatMessage->setFontItalic(isItalic);
    ui->toolButtonItalic->setDown(isItalic);
    ui->plainTextEditInput->setFocus();

}

bool ChatWindow::establishMessageConnection(quint32 peerUID)
{
    AddrMessage addr;
    QTcpSocket *socket;

    if(g_dataPool.isConnected(friendInfo.account, Connection::message_connection))
        return true;
    if(!api.getIpAndPort(Utils::getInstance()->getTcpSocket(), peerUID, addr))
        return false;
    socket = new QTcpSocket();
    socket->connectToHost(addr.ipAddr, addr.port);
    if(!socket->waitForConnected())
        return false;
    g_dataPool.setMessageConnection(socket, me.account, peerUID);
    return true;
}

void ChatWindow::on_pushButtonSend_clicked()
{
    QString temp = ui->plainTextEditInput->toPlainText();

    if(!temp.isEmpty())
    {
        if(!establishMessageConnection(friendInfo.account))
        {
            QMessageBox::warning(this, "提示", "对方可能不在线，无法发送消息");
            return;
        }
        g_dataPool.sendMessage(friendInfo.account, temp);
        ui->plainTextEditChatMessage->append(temp);
        ui->plainTextEditInput->setPlainText("");
    }
}

void ChatWindow::newMessage(quint32 peerUID, QString msg)
{
    if(peerUID == friendInfo.account)
        ui->plainTextEditChatMessage->append(msg);
}
