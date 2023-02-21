#include "basesock.h"
#include<queue>
using namespace std;

BaseSock::BaseSock(QObject *parent) : QObject(parent),onConnected([=](){})
{
    sock = new QTcpSocket();
}

BaseSock::~BaseSock()
{
    sock->disconnectFromHost();
    sock->close();
    delete sock;
}

void BaseSock::connectToHost(const QString &addr, quint16 port,QIODevice::OpenMode mode)
{
    sock->connectToHost(addr,port,mode);
    connect(sock, SIGNAL(connected()), this, SLOT(handConnected()));
}

void BaseSock::addCallback(string cmdType, std::function<void (const Json::Value &)> fun)
{
    _callbacks[cmdType] = fun;
}

quint64 BaseSock::sendMessage(const QString &msg)
{
    return sendMsg(sock, msg.toUtf8());
}

void BaseSock::setConnectCallback(std::function<void()> onConnected)
{
    this->onConnected = onConnected;
}

void BaseSock::handConnected()
{
    onConnected();
    qDebug()<< "connected successed";
    connect(sock, SIGNAL(readyRead()), this, SLOT(handReadyRead()));
}

QByteArray getData()
{

}

void BaseSock::handReadyRead()
{
    QByteArray buf = sock->readAll();
    buffer.append(buf);
    uint msgLen;
    uint tot = buffer.size();
    while(tot)
    {
        QDataStream pack(buffer);
        pack.setByteOrder(QDataStream::BigEndian);
        QByteArray msg;
        if(tot<4)
        {
            break;
        }
        pack>>msg;
        if(msg.length()==0) break;
        msgLen = msg.length() + sizeof(uint);
        Json::Reader reader;
        Json::Value json;
        qDebug()<<"recv = "<<msg;
        if (!reader.parse(msg.toStdString(), json))
        {
            qDebug() << "服务器返回信息错误";
            return;
        }
        if (!json.isObject())
        {
            qDebug() << "服务器返回信息错误";
            return;
        }
        string type = json["type"].asString();
        Json::Value cmd = json["info"];
        auto callback = _callbacks.find(type);
        if(callback!=_callbacks.end())
        {
            callback->second(cmd);
        }
        else
        {
            qDebug()<<"服务器返回信息错误";
        }
        buf = buffer.right(tot - msgLen);
        tot = buf.size();
        buffer = buf;
    }
}

void BaseSock::abort()
{
    sock->abort();
}
