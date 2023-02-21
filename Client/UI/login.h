#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QTcpSocket>
#include <QMessageBox>
#include <QDebug>
#include "basewidget.h"
#include "user.h"
#include "regist.h"
#include "utils/communicate_utils.h"
#include "socket/datasock.h"
namespace Ui
{
    class Login;
}

class Login : public BaseWidget
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = 0);
    ~Login();

private slots:
    void on_closeButton_clicked();

    void on_hideButton_clicked();

    void on_loginButton_clicked();

    void on_registButton_clicked();

    void on_forgetButton_clicked();

    void on_netSetButton_clicked();

private:
    Ui::Login *ui;
    DataSock &client;
    void showMainWindow(QString udata);
    void showRegistWindow();
    void showForgotPwdWindow();
    MainWindow *mw;
    Regist *rw;
    user *u;
};

#endif // LOGIN_H
