#ifndef GROUP_H
#define GROUP_H

#include <QWidget>
#include <QTcpSocket>
#include <QMessageBox>
#include <QDebug>
#include <QList>
#include <QTreeWidget>
#include <QCheckBox>
#include "friendlist.h"
#include "utils/communicate_utils.h"
#include "socket/datasock.h"

namespace Ui {
class group;
}

class group : public QWidget
{
    Q_OBJECT

public:
    explicit group(Ui::FriendList *f,QString ud, QWidget *parent = 0);
    ~group();

private slots:
    void on_pushButton_clicked();
    void anystateChanged(int __n);

private:
    Ui::group *ui;
    DataSock &client;
    QString ud;
    QList<int> selectedItems;
    Ui::FriendList *friendlist;
};

#endif // GROUP_H
