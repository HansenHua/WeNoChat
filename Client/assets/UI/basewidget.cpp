#include "basewidget.h"
#include <QMouseEvent>
BaseWidget::BaseWidget(QWidget *parent) : QWidget(parent)
{
    //去窗口边框
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    //把窗口背景设置为透明;
    setAttribute(Qt::WA_TranslucentBackground);
}

void BaseWidget::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
    {
        //求坐标差值
        //当前点击坐标-窗口左上角坐标
        __p = e->globalPos() - this->frameGeometry().topLeft();
    }
}

void BaseWidget::mouseMoveEvent(QMouseEvent *e)
{
    if (e->buttons() & Qt::LeftButton && __p.x() != 0 && __p.y() != 0)
    {
        //移到左上角
        move(e->globalPos() - __p);
    }
}

void BaseWidget::mouseReleaseEvent(QMouseEvent *event)
{
    __p.setX(0);
    __p.setY(0);
}
