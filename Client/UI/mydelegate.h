#ifndef MYDELEGATE_H
#define MYDELEGATE_H

#include <QWidget>
#include <QStyledItemDelegate>

class MyDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit MyDelegate(QWidget *parent = nullptr);

    void paint(QPainter *painter,
               const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const override;
};

#endif // MYDELEGATE_H
