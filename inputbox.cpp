#include "inputbox.h"

InputBox::InputBox(QTextEdit *parent) :
    QTextEdit(parent)
{
}

void InputBox::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Return && e->modifiers() == Qt::NoModifier)
    {
        e->ignore();
        emit returnPressed();
    }
    else
        QTextEdit::keyPressEvent(e);
}
