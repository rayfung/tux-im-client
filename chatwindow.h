#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include "datatype.h"

namespace Ui {
class ChatWindow;
}

class ChatWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ChatWindow(FriendMessage friendInfo, QWidget *parent = 0);
    ~ChatWindow();

private slots:
    void on_pushButtonChooseFile_clicked();
    void on_toolButtonClear_clicked();
    void on_fontComboBox_currentFontChanged(const QFont &f);
    void on_pushButtonClose_clicked();
    void on_comboBoxFontSize_currentIndexChanged(const QString &arg1);
    void on_toolButtonBoldToo_clicked();
    void on_toolButtonItalic_clicked();
    void on_pushButtonSend_clicked();

private:
    Ui::ChatWindow *ui;
    static const int oneKB = 1024;
    static const int oneMB = 1024 * 1024;
    static  const int oneGB = 1024 * 1024 * 1024;
    bool isBold;
    bool isItalic;
    QString fileName;
    FriendMessage friendInfo;
};

#endif // CHATWINDOW_H
