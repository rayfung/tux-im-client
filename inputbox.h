#ifndef INPUTBOX_H
#define INPUTBOX_H

#include <QTextEdit>
#include <QKeyEvent>

class InputBox : public QTextEdit
{
    Q_OBJECT
public:
    explicit InputBox(QTextEdit *parent = 0);

private:
    void keyPressEvent(QKeyEvent *e);

signals:
    void returnPressed();

public slots:

};

#endif // INPUTBOX_H
