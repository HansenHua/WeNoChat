#ifndef USER_H
#define USER_H

#include <QWidget>
#include <QTcpSocket>
#include <QMessageBox>
#include <QDebug>
#include "question.h"
#include "socket/datasock.h"

namespace Ui {
class user;
}

class user : public QWidget
{
    Q_OBJECT

public:
    explicit user(QWidget *parent = 0);
    ~user();
protected:
    void mouseMoveEvent(QMouseEvent *e);//鼠标移动
    void mousePressEvent(QMouseEvent *e);//鼠标按下移动
    void mouseReleaseEvent(QMouseEvent *event);

private slots:
    void on_pushButton_clicked();

    void on_closeButton_clicked();

    void on_hideButton_clicked();

private:
    Ui::user *ui;
    DataSock &client;
    question *q;
    QString udata;
    QPoint p;
};

#endif // USER_H
