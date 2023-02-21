/***
 * BaseWidget
 * 程序中所有窗口的基类
 * 实现了窗口的自由拖动，也可以自定义一些其他的公共特性
 ***/
#ifndef BASEWIDGET_H
#define BASEWIDGET_H

#include <QWidget>

class BaseWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BaseWidget(QWidget *parent = 0);
protected:
    void mouseMoveEvent(QMouseEvent *e);//鼠标移动
    void mousePressEvent(QMouseEvent *e);//鼠标按下移动
    void mouseReleaseEvent(QMouseEvent *event);
signals:

public slots:

private:
    QPoint __p;
};

#endif // BASEWIDGET_H
