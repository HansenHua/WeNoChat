#include "tools.h"
#include <QDataStream>
#include <QDebug>
uint bytes2uInt(const char *bytes)
{
    uint b0 = (uint)bytes[0] & 0xFFu;
    uint b1 = (uint)bytes[1] & 0xFFu;
    uint b2 = (uint)bytes[2] & 0xFFu;
    uint b3 = (uint)bytes[3] & 0xFFu;
    return (b0 << 24) | (b1 << 16) | (b2 << 8) | b3;
}

void uInt2Bytes(uint _n, char *buf)
{
    buf[0] = 0xFFu & (_n >> 24);
    buf[1] = 0xFFu & (_n >> 16);
    buf[2] = 0xFFu & (_n >> 8);
    buf[3] = 0xFFu & (_n);
}

QByteArray recvMsg(QTcpSocket *skt)
{
    char buf[10];
    skt->read(buf, 4);
    int nb = bytes2uInt(buf);
    QByteArray bt;
    if(nb>=skt->bytesAvailable())
         bt= skt->read(nb);
    return bt;
}

quint64 sendMsg(QTcpSocket *skt, QString txt)
{
    QByteArray bt;
    bt.resize(0);
    QDataStream sendout(&bt, QIODevice::WriteOnly);
    sendout.setVersion(QDataStream::Qt_5_8);
    sendout << txt.toUtf8();
    return skt->write(bt);
}

quint64 sendMsg(QTcpSocket *skt, QByteArray txt)
{
    QByteArray bt;
    bt.resize(0);
    QDataStream sendout(&bt, QIODevice::WriteOnly);
    sendout.setVersion(QDataStream::Qt_5_8);
    sendout << txt;
    return skt->write(bt);
}

QImage ImgFromFile(const QString &filename)
{
    QImageReader reader(filename);
    reader.setDecideFormatFromContent(true);
    return reader.read();
}
