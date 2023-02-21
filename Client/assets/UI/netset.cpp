#include "netset.h"
#include "ui_netset.h"
#include "ui_windows.h"
#include "login.h"

NetSet::NetSet(QWidget *parent) :
    BaseWidget(parent),
    ui(new Ui::NetSet)
{
    ui->setupUi(this);
}

NetSet::~NetSet()
{
    delete ui;
}

void NetSet::on_closeButton_clicked()
{
    close();
}

void NetSet::on_hideButton_clicked()
{
    showMinimized();
}

void NetSet::on_cancelButton_clicked()
{
    mUi::login->show();
    hide();
}
