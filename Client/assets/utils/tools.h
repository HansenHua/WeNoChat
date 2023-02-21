#ifndef TOOLS_H
#define TOOLS_H

#include<QTcpSocket>
#include<QImageReader>
#include<QImage>
#include<QObject>
uint bytes2uInt(const char *bytes);

void uInt2Bytes(uint _n, char *buf);

QByteArray recvMsg(QTcpSocket *skt);
quint64 sendMsg(QTcpSocket *skt, QString txt);
quint64 sendMsg(QTcpSocket *skt, QByteArray txt);

QImage ImgFromFile(const QString &filename);

#endif // TOOLS_H
