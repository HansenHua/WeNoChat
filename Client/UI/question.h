#ifndef QUESTION_H
#define QUESTION_H

#include <QWidget>
#include <QTcpSocket>
#include <QMessageBox>
#include <QDebug>
#include "password.h"
#include "socket/datasock.h"

namespace Ui {
class question;
}

class question : public QWidget
{
    Q_OBJECT

public:
    explicit question(QString user, QString q, QWidget *parent=0);
    ~question();
protected:
    void mouseMoveEvent(QMouseEvent *e);//鼠标移动
    void mousePressEvent(QMouseEvent *e);//鼠标按下移动
    void mouseReleaseEvent(QMouseEvent *event);

private slots:
    void on_confirmButton_clicked();

    void on_closeButton_clicked();

    void on_hideButton_clicked();

private:
    Ui::question *ui;
    DataSock &client;
    QString qu;
    password *pass;
    QString udata;
    QPoint p;
};

#endif // QUESTION_H
