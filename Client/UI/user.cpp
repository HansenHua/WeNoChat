#include "user.h"
#include "ui_user.h"
#include <QMouseEvent>

user::user(QWidget *parent) : QWidget(parent),
                              client(DataSock::get()),
                              ui(new Ui::user)
{
    ui->setupUi(this);
    //去窗口边框
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    //把窗口背景设置为透明;
    setAttribute(Qt::WA_TranslucentBackground);
    client.addCallback("findpWord1", [=](const Json::Value &jtmp)
                       {
        std::string questions[5] = {"请选择密保问题", "你最喜欢的颜色是什么", "你最喜欢的美食是什么", "你最喜欢的电影是什么", "你认为中国最好的大学是哪所"};
        std::string username, info;
        int secureQue;
        bool state;
        if (Decoder_findpWord1(jtmp, username, state, secureQue, info) == 0)
        {
            qDebug("FindpWord1 data back from server error/n");
            return;
        }

        if (state == 1) //用户存在
        {
            // qDebug(info);
            udata = QString::fromStdString(username);
            QString Que = QString::fromStdString(questions[secureQue]);
            q = new question(udata, Que);
            q->show();
            this->close();
        }
        else
            QMessageBox::information(this, "提示", "用户不存在"); });
}
user::~user()
{
    delete ui;
}

void user::on_pushButton_clicked()
{
    QString uName = ui->userName->text();
    QString phonenum = ui->phonenumber->text();
    //发送数据协议
    std::string data = Encoder_findpWord1(uName.toStdString(), phonenum.toStdString());
    QString packData = QString::fromStdString(data);
    client.sendMessage(packData);
}

void user::on_closeButton_clicked()
{
    close();
}

void user::on_hideButton_clicked()
{
    showMinimized();
}
//！！！！以下东西不用动！！！！
void user::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
    {
        //求坐标差值
        //当前点击坐标-窗口左上角坐标
        p = e->globalPos() - this->frameGeometry().topLeft();
    }
}

void user::mouseMoveEvent(QMouseEvent *e)
{
    if (e->buttons() & Qt::LeftButton && p.x() != 0 && p.y() != 0)
    {
        //移到左上角
        move(e->globalPos() - p);
    }
}

void user::mouseReleaseEvent(QMouseEvent *event)
{
    p.setX(0);
    p.setY(0);
}
