#include "tux.h"
#include "ui_tux.h"

Tux::Tux(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Tux)
{
    ui->setupUi(this);
}

Tux::~Tux()
{
    delete ui;
}

void Tux::on_findButton_clicked()
{
}
