#include "login.h"
#include "ui_login.h"
#include <QMouseEvent>
#include "constants.h"
#include <QPainter>
#include <QFileInfo>
#include <string>
#include "utils/tools.h"
#include "netset.h"
#include "ui_windows.h"
using namespace std;

Login::Login(QWidget *parent) : BaseWidget(parent),
                                ui(new Ui::Login),
                                client(DataSock::get())
{
    ui->setupUi(this);

    //连接服务器
    client.connectToHost(Net::addr, Net::port);

    //成功连接服务器
    client.setConnectCallback([=]() {});

    //读取服务器内容
    client.addCallback("login", [=](const Json::Value &jtmp)
                       {
        std::string username, info;
        bool state;
        if (Decoder_login(jtmp, username, info, state) == 0)
        {
            qDebug("Login data back from server error/n");
            return;
        }

        if (state == 1) //登陆成功
        {
            // qDebug(info);
            showMainWindow(QString::fromStdString(username));
            this->close();
        }
        else
            QMessageBox::information(this, "提示", info.c_str()); });
}

void Login::on_loginButton_clicked()
{
    //获取输入框内容
    QString userName = ui->userEdit->text();
    QString userPassword = ui->passwordEdit->text();
    //向服务器发送登录数据
    //发送数据协议
    std::string data = Encoder_login(userName.toStdString(), userPassword.toStdString());
    QString packData = QString::fromStdString(data);
    client.sendMessage(packData);
    //    showMainWindow();
}

//新窗体相关函数
//打开聊天主界面
void Login::showMainWindow(QString udata)
{
    mw = new MainWindow(udata);
    mw->show();
}
//打开注册界面
void Login::showRegistWindow()
{
    // TODO：这里检查一下禁止多开会更好，但是好像不能通过判断NULL来检查
    rw = new Regist();
    rw->show();
}
//打开忘记密码界面
void Login::showForgotPwdWindow()
{
    // TODO：这里检查一下禁止多开会更好，但是好像不能通过判断NULL来检查
    u = new user();
    u->show();
}

Login::~Login()
{
    delete ui;
}

void Login::on_closeButton_clicked()
{
    this->close();
}

void Login::on_hideButton_clicked()
{
    showMinimized();
}

void Login::on_registButton_clicked()
{
    showRegistWindow();
    this->close();
}

void Login::on_forgetButton_clicked()
{
    showForgotPwdWindow();
    this->close();
}
void Login::on_netSetButton_clicked()
{
    mUi::netset->show();
    hide();
}
