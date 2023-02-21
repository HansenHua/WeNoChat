#include "filesock.h"
#include <QDebug>
#include <QFileInfo>
#include <string>
#include <QDir>
using namespace std;

FileSock::FileSock(QObject *parent) : BaseSock(parent), file(NULL)
{
}

FileSock::~FileSock()
{
    if (file != NULL)
        delete file;
}

void FileSock::handReadyRead()
{
    QByteArray buf = recvMsg(sock);
    Json::Reader reader;
    Json::Value json;
    reader.parse(QString::fromUtf8(buf).toStdString(), json);
    string type = json["type"].asString();
    Json::Value cmd = json["info"];

    if (type == "sendState")
    {
        int state = cmd["state"].asInt();
        string server_name = cmd["filename"].asString();
        file->close();
        if (state)
            onSendSuccess(this, fileName, QString::fromStdString(server_name));
        sock->disconnectFromHost();
        sock->close();
        emit finished();
    }
    else if (type == "sendFile")
    {
        int state = cmd["state"].asInt();
        qint64 size = cmd["size"].asInt64();
        qDebug(Json::FastWriter().write(cmd).c_str());
        if (state == 0)
        {
            qDebug() << "文件不存在";
        }
        else
        {
            task.totalBytes = size;
            task.bytesWritten = 0;
            task.bytestoWrite = size;
            disconnect(sock, SIGNAL(readyRead()), 0, 0);
            connect(sock, SIGNAL(readyRead()), this, SLOT(upgradeRecvProgress()));
            if (sock->bytesAvailable() > 0)
            {
                upgradeRecvProgress();
            }
        }
    }
}

void FileSock::setSendCallback(std::function<void(FileSock *, const QFileInfo &fileInfo, const QString &serverFileName)> onSuccess)
{
    this->onSendSuccess = onSuccess;
}
void FileSock::setRecvCallback(std::function<void(FileSock *, const QFileInfo &fileInfo)> onSuccess)
{
    this->onRecvSuccess = onSuccess;
}

void FileSock::sendFile(const QString &uName, const QString &fileName, const QString &defaultName)
{
    this->fileName = fileName;
    file = new QFile(fileName);
    if (!file->open(QFile::ReadOnly))
    {
        qDebug() << "file " << fileName << " open error";
        return;
    }

    Json::Value sdf;
    sdf["type"] = "sendFile";
    sdf["info"]["username"] = uName.toStdString();
    QString fileAbsName = fileName.mid(fileName.lastIndexOf("/") + 1);
    int pos = fileAbsName.lastIndexOf(".");
    QString ext = fileAbsName.mid(pos);
    if (defaultName.length() != 0)
    {
        fileAbsName = defaultName + ext;
        sdf["info"]["default"] = 1;
    }
    else
        sdf["info"]["default"] = 0;
    sdf["info"]["fileName"] = fileAbsName.toStdString();
    sdf["info"]["size"] = file->size();
    sendMessage(QString::fromStdString(Json::FastWriter().write(sdf)));
    task.totalBytes = file->size();
    task.bytestoWrite = task.totalBytes;
    task.bytesWritten = 0;
    outBlock.resize(0);
    qDebug() << "start to send";
    upgradeSendProgress(4);
    connect(sock, SIGNAL(bytesWritten(qint64)), this, SLOT(upgradeSendProgress(qint64)));
}

void FileSock::upgradeSendProgress(qint64 bytenum)
{
    // 已经发送的数据大小
    task.bytesWritten += bytenum - 4;
    qDebug() << "delta = " << bytenum - 4;
    qDebug() << "byteswritten " << task.bytesWritten;

    // 如果还有数据未发送

    if (task.bytestoWrite > 0)
    {
        outBlock = file->read(qMin(task.bytestoWrite, BLOCKSIZE));
        // 发送完一次数据后还剩余数据的大小
        quint32 size = sendMsg(sock, outBlock) - 4;
        qDebug() << "blocksize" << size;
        task.bytestoWrite -= size;
        // 清空发送缓冲区
        outBlock.resize(0);
    }
    else
    {
        file->close();
    }

    // 如果发送完毕
    if (task.bytesWritten == task.totalBytes)
    {
        qDebug() << "send over" << endl;
        file->close();
    }
}

void FileSock::rqirFile(const QString &fileName, const QString &path, const QString &defaultName)
{
    if (fileName.length() == 0)
    {
        onRecvSuccess(this, fileName);
        sock->disconnectFromHost();
        sock->close();
        emit finished();
        return;
    }
    QDir dir;
    if (!dir.exists(path + "/"))
        dir.mkpath(path + "/");
    Json::Value json;
    json["type"] = "rqirFile";
    json["info"]["filename"] = fileName.toStdString();
    qDebug() << QString::fromStdString(Json::FastWriter().write(json));
    QStringList paths = fileName.split("/");
    QString absfileName = paths.at(paths.length() - 1);
    int posd = absfileName.lastIndexOf('.');
    QString fname, ext;
    if (posd == -1)
    {
        fname = absfileName;
        ext = "";
    }
    else
    {
        fname = absfileName.mid(0, posd);
        ext = absfileName.mid(posd);
    }
    QString fTmpName;
    if (defaultName.length() != 0)
    {
        fTmpName = defaultName + ext;
    }
    else
    {
        int idx = 1;
        fTmpName = fname + ext;
        do
        {
            if (!QFileInfo(path + "/" + fTmpName).isFile())
                break;
            while (QFileInfo(path + "/" + (fTmpName = fname + "(" + QString::number(idx) + ")" + ext)).isFile())
                idx++;
        } while (false);
    }
    this->fileName = path + "/" + fTmpName;
    qDebug() << this->fileName;
    this->file = new QFile(this->fileName);
    if (!this->file->open(QFile::WriteOnly))
    {
        qDebug() << "create file error";
        return;
    }
    sendMessage(QString::fromStdString(Json::FastWriter().write(json)));
}

void FileSock::upgradeRecvProgress()
{
    qDebug() << "receive";
    if (task.bytesWritten < task.bytestoWrite)
    {
        task.bytesWritten += sock->bytesAvailable();
        inBlock = sock->readAll();
        qDebug() << "blocksize = " << inBlock.size();
        file->write(inBlock);
        inBlock.resize(0);
    }
    qDebug() << "byteswritten " << task.bytesWritten;
    if (task.bytesWritten == task.totalBytes)
    {
        qDebug() << "receive succeeded";
        file->close();
        onRecvSuccess(this, this->fileName);
        sock->disconnectFromHost();
        sock->close();
        qDebug()<<"prepare to emit";
        emit finished();
    }
}

FileSock *createRequireTask(const QString &fileName, const QString &path,
                            std::function<void(FileSock *, const QFileInfo &fileInfo)> onSuccess, const QString &defaultName,
                            const QString &addr, quint16 port, QIODevice::OpenMode mode)
{
    FileSock *filesock = new FileSock;
    filesock->setRecvCallback(onSuccess);
    filesock->connectToHost(addr, port, mode);
    filesock->rqirFile(fileName, path, defaultName);
    return filesock;
}

FileSock *createSendTask(const QString &userName, const QString &fileName,
                         std::function<void(FileSock *, const QFileInfo &fileInfo, const QString &serverFileName)> onSuccess,
                         const QString &defaultName, const QString &addr, quint16 port, QIODevice::OpenMode mode)
{
    FileSock *filesock = new FileSock;
    filesock->setSendCallback(onSuccess);
    filesock->connectToHost(addr, port);
    filesock->sendFile(userName, fileName, defaultName);
    return filesock;
}
