#ifndef NETSET_H
#define NETSET_H

#include "basewidget.h"

namespace Ui {
class NetSet;
}

class NetSet : public BaseWidget
{
    Q_OBJECT

public:
    explicit NetSet(QWidget *parent = 0);
    ~NetSet();

private slots:
    void on_closeButton_clicked();

    void on_hideButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::NetSet *ui;
};

#endif // NETSET_H
