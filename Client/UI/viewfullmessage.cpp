#include "viewfullmessage.h"
#include "ui_viewfullmessage.h"

ViewFullMessage::ViewFullMessage(QWidget *parent) : QWidget(parent),
                                                    ui(new Ui::ViewFullMessage)
{
    ui->setupUi(this);
    this->setWindowTitle("暂无消息");
    ui->textEdit->setText("");
}

ViewFullMessage::ViewFullMessage(QString strTxt, QWidget *parent) : QWidget(parent),
                                                                    ui(new Ui::ViewFullMessage)
{
    ui->setupUi(this);
    this->setWindowTitle("详细消息显示");
    ui->textEdit->setText(strTxt);
}

ViewFullMessage::~ViewFullMessage()
{
    delete ui;
}

void ViewFullMessage::resizeEvent(QResizeEvent *event)
{
    this->ui->textEdit->resize(this->width(), this->height());
    this->ui->textEdit->move(0, 0);
}
