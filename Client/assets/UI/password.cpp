#include "password.h"
#include "ui_password.h"
#include <QMouseEvent>

password::password(QString u, QWidget *parent) : QWidget(parent),
                                                 client(DataSock::get()),
                                                 ui(new Ui::password)
{
    ui->setupUi(this);
    //去窗口边框
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    //把窗口背景设置为透明;
    setAttribute(Qt::WA_TranslucentBackground);
    uname = u;
    client.addCallback("findpWord3", [=](const Json::Value &jtmp)
                       {
        std::string username;
        bool state;
        if (Decoder_findpWord3(jtmp, username, state) == 0)
        {
            qDebug("Regist data back from server error/n");
            return;
        }
        if (state == 1) //注册成功
        {
            // qDebug(info);
            mw = new MainWindow(uname);
            mw->show();
            this->close();
        }
        else
            QMessageBox::information(this, "提示", "密码更新失败"); });
}

password::~password()
{
    delete ui;
}

void password::on_confirm_clicked()
{
    QString pwd = ui->passw->text();
    QString pwd_2 = ui->passw_2->text();
    qDebug() << pwd << pwd_2;
    //两次密码相同
    if (pwd == pwd_2)
    {
        //与服务器连接，检查注册
        std::string data = Encoder_findpWord3(uname.toStdString(), pwd.toStdString());
        QString packData = QString::fromStdString(data);
        client.sendMessage(packData);
    }
    else
    {
        QMessageBox::information(this, "提示", "密码不一致");
    }
}

void password::on_closeButton_clicked()
{
    close();
}

void password::on_hideButton_clicked()
{
    showMinimized();
}
//！！！！以下东西不用动！！！！
void password::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
    {
        //求坐标差值
        //当前点击坐标-窗口左上角坐标
        p = e->globalPos() - this->frameGeometry().topLeft();
    }
}

void password::mouseMoveEvent(QMouseEvent *e)
{
    if (e->buttons() & Qt::LeftButton && p.x() != 0 && p.y() != 0)
    {
        //移到左上角
        move(e->globalPos() - p);
    }
}

void password::mouseReleaseEvent(QMouseEvent *event)
{
    p.setX(0);
    p.setY(0);
}
