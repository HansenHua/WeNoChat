#include "searchfriends.h"
#include "ui_searchfriends.h"
#include <QMouseEvent>

searchFriends::searchFriends(QString u, QWidget *parent) : QDialog(parent),
                                                           client(DataSock::get()),
                                                           ui(new Ui::searchFriends)
{
    ui->setupUi(this);
    //设置不可改大小
    this->setFixedSize(this->geometry().size());
    //去窗口边框
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    udata = u;
}

searchFriends::~searchFriends()
{
    delete ui;
}

void searchFriends::on_pushButton_clicked()
{
    //获取输入框内容
    QString friend_userName = ui->lineEdit->text();
    //向服务器发送新好友
    std::string data = Encoder_addfriends(udata.toStdString(), friend_userName.toStdString());
    QString packData = QString::fromStdString(data);
    client.sendMessage(packData);
    close();
}

void searchFriends::on_closeButton_clicked()
{
    close();
}

void searchFriends::on_hideButton_clicked()
{
    showMinimized();
}
//！！！！以下东西不用动！！！！
void searchFriends::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
    {
        //求坐标差值
        //当前点击坐标-窗口左上角坐标
        p = e->globalPos() - this->frameGeometry().topLeft();
    }
}

void searchFriends::mouseMoveEvent(QMouseEvent *e)
{
    if (e->buttons() & Qt::LeftButton && p.x() != 0 && p.y() != 0)
    {
        //移到左上角
        move(e->globalPos() - p);
    }
}

void searchFriends::mouseReleaseEvent(QMouseEvent *event)
{
    p.setX(0);
    p.setY(0);
}
