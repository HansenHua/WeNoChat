#ifndef BASESOCK_H
#define BASESOCK_H

#include <QObject>
#include <QIODevice>
#include <QTcpSocket>
#include <QDebug>
#include <string>
#include <functional>
#include <utility>
#include <utils/json/json.h>
#include"utils/tools.h"

class BaseSock : public QObject
{
    Q_OBJECT
public:
    explicit BaseSock(QObject *parent = 0);
    quint64 sendMessage(const QString &msg);
    void connectToHost(const QString &addr, quint16 port,QIODevice::OpenMode mode = QIODevice::ReadWrite);
    void addCallback(std::string cmdType, std::function<void(const Json::Value &cmd)> fun);
    void setConnectCallback(std::function<void()> onConnected);
    void abort();
    virtual ~BaseSock();
public slots:
    void handConnected();
    virtual void handReadyRead();
protected:
    QTcpSocket *sock;
    std::map<std::string, std::function<void(const Json::Value &cmd)>> _callbacks;
    std::function<void()> onConnected;
    QByteArray buffer;
};

#endif // BASESOCK_H
