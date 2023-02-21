#include "mydelegate.h"
#include <QPainter>
#include <QApplication>

MyDelegate::MyDelegate(QWidget *parent) : QStyledItemDelegate(parent) {}

void MyDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);
    if (!index.parent().isValid())//一级节点
    {
        opt.decorationSize = QSize(20, 20);//设置图标大小 根据需求修改数值
    }
    else
    {
        opt.decorationSize = QSize(40, 40);
    }
    const QWidget *widget = NULL;
    widget = option.widget;
    QStyle *style = widget ? widget->style() : QApplication::style();
    style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, widget);
}


QSize MyDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (!index.parent().isValid())//一级节点
    {
        return QSize(30, 30);
    }
    else
    {
        return QSize(60, 60);
    }
    return QStyledItemDelegate::sizeHint(option, index);
}
