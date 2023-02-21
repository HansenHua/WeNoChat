#ifndef VIEWFULLMESSAGE_H
#define VIEWFULLMESSAGE_H

#include <QWidget>

namespace Ui
{
    class ViewFullMessage;
}

class ViewFullMessage : public QWidget
{
    Q_OBJECT

public:
    explicit ViewFullMessage(QWidget *parent = 0);
    explicit ViewFullMessage(QString strTxt, QWidget *parent = 0);
    ~ViewFullMessage();

protected:
    void resizeEvent(QResizeEvent *event);

private:
    Ui::ViewFullMessage *ui;
};

#endif // VIEWFULLMESSAGE_H
