#include "question.h"
#include "ui_question.h"
#include <QMouseEvent>

question::question(QString user, QString q, QWidget *parent) : QWidget(parent),
                                                               client(DataSock::get()),
                                                               ui(new Ui::question)
{
    udata = user;
    ui->setupUi(this);
    //去窗口边框
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    //把窗口背景设置为透明;
    setAttribute(Qt::WA_TranslucentBackground);
    qu = q;
    ui->question_2->setText(qu);
    client.addCallback("findpWord2", [=](const Json::Value &jtmp)
                       {
        std::string username;
        bool state;
        if (Decoder_findpWord2(jtmp, username, state) == 0)
        {
            qDebug("FindpWord2 data back from server error/n");
            return;
        }
        if (state == 1) //注册成功
        {
            // qDebug(info);
//            disconnect(client, SIGNAL(readyRead()), 0, 0);
            pass = new password(udata);
            pass->show();
            this->close();
        }
        else
            QMessageBox::information(this, "提示", "密保问题错误"); });
}

question::~question()
{
    delete ui;
}

void question::on_confirmButton_clicked()
{
    QString ans = ui->answer->text();
    //发送数据协议
    std::string data = Encoder_findpWord2(udata.toStdString(), ans.toStdString());
    QString packData = QString::fromStdString(data);
    client.sendMessage(packData);
}

void question::on_closeButton_clicked()
{
    close();
}

void question::on_hideButton_clicked()
{
    showMinimized();
}
//！！！！以下东西不用动！！！！
void question::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
    {
        //求坐标差值
        //当前点击坐标-窗口左上角坐标
        p = e->globalPos() - this->frameGeometry().topLeft();
    }
}

void question::mouseMoveEvent(QMouseEvent *e)
{
    if (e->buttons() & Qt::LeftButton && p.x() != 0 && p.y() != 0)
    {
        //移到左上角
        move(e->globalPos() - p);
    }
}

void question::mouseReleaseEvent(QMouseEvent *event)
{
    p.setX(0);
    p.setY(0);
}
