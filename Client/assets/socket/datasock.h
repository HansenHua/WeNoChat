#ifndef DATASOCK_H
#define DATASOCK_H

#include"basesock.h"


//注意，这是个单例类。因为全局用来收发消息的socket只有一个。
class DataSock : public BaseSock
{
    Q_OBJECT
public:
    static DataSock &get(QObject *parent = 0);
private:
    explicit DataSock(QObject *parent = 0);
    DataSock(const DataSock &)=delete;
    DataSock &operator=(const DataSock &)=delete;
};

#endif // DATASOCK_H
