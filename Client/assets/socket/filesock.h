#ifndef FILESOCK_H
#define FILESOCK_H

#include <QObject>
#include <QTcpSocket>
#include <QFile>
#include<QFileInfo>
#include <QDataStream>
#include <functional>
#include "constants.h"
#include "basesock.h"

struct FileTask
{
    qint64 totalBytes;      //文件总字节数
    qint64 bytesWritten;    //已发送的字节数
    qint64 bytestoWrite;    //尚未发送的字节数
};

class FileSock : public BaseSock
{
    Q_OBJECT
public:
    explicit FileSock(QObject *parent = 0);
    ~FileSock();
    void createFile(const QString &fileName);
    void sendFile(const QString &userName,  const QString &fileName, const QString &defaultName = "");

    /**
     * @brief 从服务端请求在服务端的路径为fileName的文件，并下载到path文件夹
     * @param fileName 服务端的文件路径（需要提前知道）
     * @param path 要下载到的地方
     **/
    void rqirFile(const QString &fileName, const QString &path, const QString &defaultName = "");
    void setSendCallback(std::function<void(FileSock *, const QFileInfo &fileInfo, const QString &serverFileName)> onSuccess);
    void setRecvCallback(std::function<void(FileSock *, const QFileInfo &fileInfo)> onSuccess);
private slots:
    void upgradeSendProgress(qint64 bytenum);
    void upgradeRecvProgress();
    void handReadyRead();
signals:
    finished();

private:
    QString fileName;

    QFile *file;

    FileTask task;

    QByteArray inBlock;
    QByteArray outBlock;

    std::function<void(FileSock *, const QFileInfo &fileInfo, const QString &serverFileName)> onSendSuccess;
    std::function<void(FileSock *, const QFileInfo &fileInfo)> onRecvSuccess;

    const qint64 BLOCKSIZE = 4096;

    bool running = false;
};

FileSock *createRequireTask(const QString &fileName, const QString &path, std::function<void(FileSock *,const QFileInfo &)> onSuccess,
                       const QString &defaultName = "", const QString &addr = Net::addr, quint16 port = Net::filePort, QIODevice::OpenMode mode = QIODevice::ReadWrite);


FileSock *createSendTask(const QString &userName, const QString &fileName,
                    std::function<void(FileSock *, const QFileInfo &, const QString &)> onSuccess,
                    const QString &defaultName = "",
                    const QString &addr = Net::addr, quint16 port= Net::filePort,  QIODevice::OpenMode mode = QIODevice::ReadWrite);

#endif // FILESOCK_H
