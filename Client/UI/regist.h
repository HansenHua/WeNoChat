#ifndef REGIST_H
#define REGIST_H

#include <QWidget>
#include <QTcpSocket>
#include <QMessageBox>
#include <QDebug>
#include"utils/communicate_utils.h"
#include "mainwindow.h"
#include "socket/datasock.h"
namespace Ui {
class Regist;
}

class Regist : public QWidget
{
    Q_OBJECT

public:
    explicit Regist(QWidget *parent = 0);
    ~Regist();

protected:
    void mouseMoveEvent(QMouseEvent *e);//鼠标移动
    void mousePressEvent(QMouseEvent *e);//鼠标按下移动
    void mouseReleaseEvent(QMouseEvent *event);

private slots:
    void on_loginButton_clicked();

    void on_closeButton_clicked();

    void on_hideButton_clicked();

private:
    void showMainWindow(const QString &udata);
    Ui::Regist *ui;
    MainWindow * mw;
    DataSock &client;
    QString udata;
    QPoint p;
};

#endif // REGIST_H
