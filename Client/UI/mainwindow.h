#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QTcpSocket>
#include <QListWidgetItem>
#include <QMainWindow>
#include "picturecut.h"
#include <UI/chatmessagewidget.h>
#include <QDateTime>
#include "utils/communicate_utils.h"
#include "searchfriends.h"
#include "friendlist.h"
#include <functional>
#include <QImage>
#include <UI/group.h>
#include <set>
#include "messagerecord.h"
#include "socket/datasock.h"
namespace Ui
{
    class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    explicit MainWindow(QString ud, QWidget *parent = 0);
    ~MainWindow();
    void pushFileIntoChatWindow(bool type,QString filePath, QString time, QImage *image = NULL, bool isSending = false);
    void dealFile(ChatMessageWidget *messageW, QListWidgetItem *item,QString filePath,QString msg, QString time, ChatMessageWidget::User_Type type, QImage *image);
    void pushImageIntoChatWindow(bool type, QImage msg, QString time, QImage *image = NULL, bool isSending = false);
    void dealImage(ChatMessageWidget *messageW, QListWidgetItem *item, QImage img, QString time, ChatMessageWidget::User_Type type, QImage *image);
    void changeMyIcon(QImage *uimg);
    void changeIcon(QString username, QImage icon);
protected:
    void mouseMoveEvent(QMouseEvent *ev);  //鼠标移动
    void mousePressEvent(QMouseEvent *ev); //鼠标按下移动
    void mouseReleaseEvent(QMouseEvent *ev);
    void MoveFps();//收到当前聊天框新消息抖动
private slots:
    void submitheadImage(Ui::headImage);
    void on_closeButton_clicked();

    void on_hideButton_clicked();

    void on_pushButton_addfriend_clicked();
    void on_send_clicked();

    void pushMessageIntoChatWindow(bool type, QString msg, QString time, QImage *image = NULL, bool isSending = false);
    void clearAllMessage();
    void dealMessageTime(QString curMsgTime);
    void dealMessage(ChatMessageWidget *messageW, QListWidgetItem *item, QString text, QString time, ChatMessageWidget::User_Type type, QImage *image = NULL);

    void startChatting(QVariant variant, int a);
    void deleteFriend(QVariant variant);
    void on_pushButton_image_clicked();
    void on_pushButton_send_image_clicked();
    void on_biaoqingButton_clicked();

    void on_bButton1_clicked();
    void on_bButton2_clicked();
    void on_bButton3_clicked();
    void on_bButton4_clicked();
    void on_bButton5_clicked();
    void on_bButton6_clicked();
    void on_bButton7_clicked();
    void on_bButton8_clicked();
    void on_bButton9_clicked();
    void on_pushButton_3_clicked();

    void on_pushButton_addgroup_clicked();

signals:
    all_finished();

private:
    struct ChatInfo
    {
        std::vector<Ui::User> chatFriend;
        MessageRecord *record;
    };
    bool isgroup;
    int groupid;

    Ui::MainWindow *ui;
    DataSock &client;
    QString udata;
    QImage user_image;
    searchFriends *add;

    Ui::FriendList *friendList;
    ChatInfo chattingInfo;
    void initConnection();

    std::map <QString, MessageRecord> msgRcd;

    QString tmpCmd;
    QPoint p;

    void _initHandler();

    void sendChatImage(const QString &imgFile, std::vector<std::string> userList, std::function<void()>onSuccess);
};

#endif // MAINWINDOW_H
