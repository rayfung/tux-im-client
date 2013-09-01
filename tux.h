#ifndef TUX_H
#define TUX_H

#include <QWidget>

namespace Ui {
class Tux;
}

class Tux : public QWidget
{
    Q_OBJECT
    
public:
    explicit Tux(QWidget *parent = 0);
    ~Tux();
    
private slots:
    void on_findButton_clicked();

private:
    Ui::Tux *ui;
};

#endif
