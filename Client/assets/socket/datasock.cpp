#include "datasock.h"

using namespace std;

DataSock::DataSock(QObject *parent) : BaseSock(parent)
{}

DataSock &DataSock::get(QObject *parent)
{
    static DataSock singleton(parent);
    return singleton;
}
