#ifndef CMD_H
#define CMD_H

#include <QObject>

class Cmd : public QObject
{
    Q_OBJECT
public:
    explicit Cmd(QObject *parent = 0);

};

#endif // CMD_H
