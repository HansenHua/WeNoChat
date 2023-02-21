#ifndef MESSAGERECORD_H
#define MESSAGERECORD_H

#include <string>
#include <vector>
#include <QImage>

struct MessageInfo
{
    QString Sender;
    QString time;
    QString msg;
    QImage img;
    QString filePath;
    enum type_t{
        PLAIN_TXT,
        IMAGE,
        FILE
    } type;
    MessageInfo(const QString &_sdr, const QString &_msg, const QString _time, type_t _type):
        Sender(_sdr),
        time(_time)
    {
        type = _type;
        if(_type == PLAIN_TXT) msg = _msg;
        else filePath = _msg;
    }
    MessageInfo(const QString &_sdr, const QImage &_img, const QString _time, type_t _type):
        Sender(_sdr),
        time(_time)
    {
        type = _type;
        img = _img;
    }
    friend bool operator<(const MessageInfo &a, const MessageInfo &b)
    {
        return a.time.toULongLong() < b.time.toULongLong();
    }
};

typedef std::vector<MessageInfo> MessageRecord;

#endif // MESSAGERECORD_H
