#ifndef PASSWORD_H
#define PASSWORD_H

#include <QWidget>
#include <QTcpSocket>
#include <QMessageBox>
#include <QDebug>
#include "mainwindow.h"
#include "socket/datasock.h"

namespace Ui {
class password;
}

class password : public QWidget
{
    Q_OBJECT

public:
    explicit password(QString u, QWidget *parent = 0);
    ~password();
protected:
    void mouseMoveEvent(QMouseEvent *e);//鼠标移动
    void mousePressEvent(QMouseEvent *e);//鼠标按下移动
    void mouseReleaseEvent(QMouseEvent *event);
private slots:
    void on_confirm_clicked();

    void on_closeButton_clicked();

    void on_hideButton_clicked();

private:
    Ui::password *ui;
    DataSock &client;
    MainWindow *mw;
    QString uname;
    QPoint p;
};

#endif // PASSWORD_H
