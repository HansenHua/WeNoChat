#ifndef CHATMESSAGEWIDGET_H
#define CHATMESSAGEWIDGET_H

#include <QWidget>
#include <QImage>
#include "utils/communicate_utils.h"
#include "viewfullmessage.h"
class QPaintEvent;
class QPainter;
class QLabel;
class QMovie;

class ChatMessageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ChatMessageWidget(QWidget *parent = nullptr);
    enum User_Type
    {
        User_System, //系统备用（预留好友确认等系统消息，也可以在User_Time里面弄）
        User_Me,     //自己
        User_She,    //对方
        User_Time,   //时间
        Image_She,
        Image_Me,
        File_She,
        File_Me

    };

    void setTextSuccess();
    void setText(QString text, QString time, QSize allSize, User_Type userType, QImage *image = NULL);
    void setImage(QImage img, QString time, ChatMessageWidget::User_Type userType, QImage *image = NULL);
    void setFile(QString filePath,QString showName,QString time,QSize allsize, ChatMessageWidget::User_Type userType, QImage *image = NULL);

    QSize getRealString(QString src);
    QSize fontRect(QString str);

    inline QString text() { return m_msg; }
    inline QString time() { return m_time; }
    inline User_Type userType() { return m_userType; }

protected:
    void paintEvent(QPaintEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *e);

private:
    ViewFullMessage *vfm;

    QString m_filePath;

    QString m_msg;
    QString m_time;
    QString m_curTime;

    QSize m_allSize;
    User_Type m_userType = User_System;

    int m_kuangWidth;
    int m_textWidth;
    int m_spaceWid;
    int m_lineHeight;

    QRect m_iconLeftRect;
    QRect m_iconRightRect;
    QRect m_sanjiaoLeftRect;
    QRect m_sanjiaoRightRect;
    QRect m_kuangLeftRect;
    QRect m_kuangRightRect;
    QRect m_textLeftRect;
    QRect m_textRightRect;
    QPixmap m_leftPixmap;
    QPixmap m_rightPixmap;
    QPixmap m_ImageMessage;
    QLabel *m_loading = Q_NULLPTR;
    QMovie *m_loadingMovie = Q_NULLPTR;
    bool m_isSending = false;
};

#endif // CHATMESSAGEWIDGET_H
