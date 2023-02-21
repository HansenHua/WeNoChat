#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMouseEvent>
#include "utils/tools.h"
#include "constants.h"
#include <vector>
#include <dirent.h>
#include "UI/picturecut.h"
#include <windows.h>
#include <QFileDialog>
#include "socket/filesock.h"
using namespace std;

MainWindow::MainWindow(QWidget *parent) : QWidget(parent),
                                          client(DataSock::get()),
                                          ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->biaoqingFrame->setVisible(false);
    tmpCmd = QString();
    //去窗口边框
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    // TODO：没有窗口边框怎么顶部拖动？继承QLabel，重写mouseMoveEvent和press...，顶部两个label就可以实现拖动窗口了
}

void MainWindow::clearAllMessage()
{
    while (ui->listWidget->count() > 0)
    {
        delete ui->listWidget->item(ui->listWidget->count() - 1);
    }
}

MainWindow::MainWindow(QString ud, QWidget *parent) : QWidget(parent),
                                                      udata(ud),
                                                      client(DataSock::get()),
                                                      ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->biaoqingFrame->setVisible(false);
    ui->indowLabel->setText(ud);
    friendList = new Ui::FriendList(ui->friendList);
    initConnection();
    _initHandler();
    //    imag = new WNCimage(ud, client);
    //去窗口边框
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    // TODO：没有窗口边框怎么顶部拖动？继承QLabel，重写mouseMoveEvent和press...，顶部两个label就可以实现拖动窗口了
    //    connect(client, SIGNAL(readyRead()), this, SLOT(hadreadyread()));
    qDebug() << "ud=" << ud;
    //发送好友列表请求
    std::string data = Encoder_askfriendsList(udata.toStdString());
    QString packData = QString::fromStdString(data);
    client.sendMessage(packData);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::_initHandler()
{

    client.addCallback("askfriendsList", [=](const Json::Value &jtmp)
                       {
        std::string username, info;
        bool state;
        std::string userimage;
        vector<pair<string, string>> uinfolist;
        if (Decoder_askfriendsList(jtmp, username, uinfolist, userimage) == 0)
        {
            qDebug("askfriends data back from server error/n");
            return;
        }
        createRequireTask(QString::fromStdString(userimage), "./assets/"+ QString::fromStdString(username), [=](FileSock *fsk, const QFileInfo &fileName){
            if(fileName.absolutePath().length()!=0)
                user_image = QImage(fileName.absoluteFilePath());
            changeMyIcon(&user_image);
        }, "head");
        std::vector<Ui::User> userList;
        for(pair<string, string> frdinfo: uinfolist)
        {
            Ui::User frd;
            frd.userName = frdinfo.first;
            frd.image = QImage();
            userList.push_back(frd);
            msgRcd[QString::fromStdString(frdinfo.first)]=MessageRecord();
        }

        friendList->initFriendList(userList);
        for(pair<string, string> frdinfo: uinfolist){

            createRequireTask(QString::fromStdString(frdinfo.second), "./assets/"+QString::fromStdString(username)+"/friendheads",
                              [=](FileSock *fsk, const QFileInfo &fileInfo){
                QImage icon;
                if(fileInfo.absolutePath().length() != 0)
                    icon.load(fileInfo.absoluteFilePath());
                changeIcon(QString::fromStdString(frdinfo.first), icon);
            }, QString::fromStdString(frdinfo.first) + "head");
        }
    });
    client.addCallback("add_group_chat", [=](const Json::Value &jtmp)
    {
        int groupid;
        std::vector<std::string> username;
        bool state;
        string groupname;
        if(Decoder_add_group_chat(jtmp, username,groupid,state, groupname) == 0)
        {
            qDebug("group_chat data back from server error/n");
            return;
        }
        Ui::Group group;
        group.groupid = groupid;
        group.member = username;
        friendList->addGroupInfo(group);
    });
    client.addCallback("chat", [=](const Json::Value &jtmp)
                       {
        std::string sender_username, msg, time;
        if (Decoder_chat(jtmp, sender_username, msg, time) == 0)
        {
            qDebug("chat data back from server error/n");
            return;
        }
        pushMessageIntoChatWindow(false, QString::fromStdString(msg), QString::number(QDateTime::currentDateTime().toTime_t()), &chattingInfo.chatFriend[0].image);
        msgRcd[QString::fromStdString(sender_username)].push_back(MessageInfo(QString::fromStdString(sender_username), QString::fromStdString(msg), QString::fromStdString(time), MessageInfo::PLAIN_TXT));
        MoveFps(); });
    client.addCallback("chat_group", [=](const Json::Value &jtmp)
                       {
        int groupid;
        std::string username, msg, time;
        if (Decoder_chat(jtmp, username, msg, time) == 0)
        {
            qDebug("chat group data back from server error/n");
            return;
        }
        Ui::User sender_;
        for(Ui::User user: chattingInfo.chatFriend)
            if(user.userName==username)
                sender_ = user;
        pushMessageIntoChatWindow(false, QString::fromStdString(msg), QString::fromStdString(time),&sender_.image);
        MoveFps(); });
    client.addCallback("addfriends", [=](const Json::Value &jtmp)
                       {
        // mainwindow里处理加好友的返回信息
        std::string friend_username;
        bool state;
        std::string friendimage;
        if (Decoder_addfriends(jtmp, friend_username, state, friendimage) == 0)
        {
            qDebug("addfriends data back from server error/n");
            return;
        }
        if (state == 1)
        {
            /*Todo:在列表里添加这个新的好友*/
            createRequireTask(QString::fromStdString(friendimage), "./assets/"+udata+"/friendheads",[=](FileSock *fsk, const QFileInfo &fileName){
                QImage friend_image;
                if(fileName.absolutePath().length()!=0)
                    friend_image.load(fileName.absoluteFilePath());
                Ui::User f;
                f.userName = friend_username;
                f.image = friend_image;
                friendList->insertToGroup("default", vector<Ui::User>{f});
                msgRcd[QString::fromStdString(friend_username)]=MessageRecord();
                QMessageBox::information(this, "提示", ("成功添加好友" + friend_username).c_str()); //
            }, QString::fromStdString(friend_username+"head"));

        }
        else
            QMessageBox::information(this, "提示", "该好友不存在"); });
    client.addCallback("chatfile", [=](const Json::Value &jtmp)
                       {
        std::string sender_username, fn, time;
        if (Decoder_chatfile(jtmp, fn, sender_username, time) == 0)
        {
            qDebug("chatfile data back from server error/n");
            return;
        }
        createRequireTask(QString::fromStdString(fn), "./assets/"+udata+"/FileRecv", [=](FileSock *sk, const QFileInfo &filesName){
            QString filename;
            QString cat;
            if(filesName.absolutePath().length()==0)
            {
                filename = "";
                qDebug()<<"fnm "<<"";
                cat="";
            }
            else
            {
                filename = filesName.absoluteFilePath();
                qDebug()<<"fnm "<<filename;
                cat = filename.right(filename.size() - filename.lastIndexOf(".") - 1);
            }

            qDebug() << "文件类型：" << cat;
            if (cat == "jpg" || cat == "jpeg" || cat == "svg" || cat == "bmp" || cat == "png" || cat == "gif")
            {
                QImage chat_image = QImage(filename);
                //处理返回来的chat_image图片信息
                pushImageIntoChatWindow(false, chat_image, QString::fromStdString(time), &chattingInfo.chatFriend[0].image);
                msgRcd[QString::fromStdString(sender_username)].push_back(MessageInfo(QString::fromStdString(sender_username), chat_image, QString::fromStdString(time), MessageInfo::IMAGE));
                MoveFps();
            }
            else
            {
                pushFileIntoChatWindow(false, filename, QString::fromStdString(time), &chattingInfo.chatFriend[0].image);
                msgRcd[QString::fromStdString(sender_username)].push_back(MessageInfo(QString::fromStdString(sender_username), filename, QString::fromStdString(time), MessageInfo::FILE));
                MoveFps();
            }
        }); });
    client.addCallback("msgrecord", [=](const Json::Value &jtmp)
                       {
        string sdr = jtmp["sender"].asString();
        string rcv = jtmp["receiver"].asString();
        int isy = jtmp["isYou"].asInt();
        string msg = jtmp["msg"].asString();
        string time = jtmp["time"].asString();
        if(isy)
        {
            msgRcd[QString::fromStdString(rcv)].push_back(MessageInfo(QString::fromStdString(sdr), QString::fromStdString(msg), QString::fromStdString(time), MessageInfo::PLAIN_TXT));   
        }
        else
        {
            msgRcd[QString::fromStdString(sdr)].push_back(MessageInfo(QString::fromStdString(sdr), QString::fromStdString(msg), QString::fromStdString(time), MessageInfo::PLAIN_TXT));
        } });
    client.addCallback("submit_image", [=](const Json::Value &jtmp){
        string username = jtmp["username"].asString();
        string imgPath = jtmp["image"].asString();
//        createRequireTask(QString::fromStdString(imgPath), "./assets/friend")
    });
}

void MainWindow::on_hideButton_clicked()
{
    showMinimized();
}

void MainWindow::on_closeButton_clicked()
{
    close();
}

void MainWindow::on_send_clicked()
{
    QString msg = ui->textEdit->toPlainText();
    if (msg.isEmpty())
        return; //空字符不发送

    QString time;
    pushMessageIntoChatWindow(true, msg, time = QString::number(QDateTime::currentDateTime().toTime_t()), &user_image, false);
    std::vector<std::string> usersList;
    for(auto v:chattingInfo.chatFriend)
        usersList.push_back(v.userName);
    QString packData;
    if(!isgroup)
    {
        msgRcd[QString::fromStdString(chattingInfo.chatFriend[0].userName)].
            push_back(MessageInfo(QString::fromStdString(chattingInfo.chatFriend[0].userName),msg,time,MessageInfo::PLAIN_TXT));
    //发送数据协议

        std::string data = Encoder_chat(udata.toStdString(), msg.toStdString(), time.toStdString(), usersList);
        packData = QString::fromStdString(data);
    }
    else
    {
        std::string data = Encoder_chat_group(udata.toStdString(), groupid, msg.toStdString(), time.toStdString());
        packData = QString::fromStdString(data);
    }
    client.sendMessage(packData);
    ui->textEdit->clear();
}

/*
 * pushMessageIntoChatWindow函数说明
 * bool type：发给别人消息，右侧气泡==true；接收到消息，左侧气泡==false。
 * QString msg：发送/接收的文本。
 * QString time：消息时间。
 *      参考：time = QString::number(QDateTime::currentDateTime().toTime_t());
 * bool isSending = false：是否正在发送，大概率用不到所以先设置成false
 *      仅发送消息有效，接收消息时这里可以随便赋值。
 * QImage* image = NULL:头像指针，默认为NULL，则显示默认头像。
 *
 * *******注意********
 * ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓特别注意↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓
 * ①   UI修改时需要全局搜索替换widthWithoutListWidget的值
 *      该值是整个mainWindow去掉ListWidget对话框的宽度
 *      不替换可能对话框显示大小异常
 * ②   TODO：检查可能有内存溢出，注意delete ui->listWidget->item(i);
 *      for循环将所有都删掉
 * ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑
 * 注意处理空消息，空消息最好别传进来，传进来也不会显示
 * 如果是发消息，注意清空输入框
 * TODO：不清楚分辨率和缩放的影响，可能会有小概率长文本显示横向拖动条
 */
void MainWindow::pushMessageIntoChatWindow(bool type, QString msg, QString time, QImage *image, bool isSending)
{

    if (msg == "")
    {
        QMessageBox::warning(this, "警告", "发送内容不能为空", QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    qDebug() << "addMessage" << msg << time << ui->listWidget->count();
    if (type)
    {
        qDebug() << "self";
        if (isSending)
        {
            qDebug() << "sending";
            dealMessageTime(time);
            ChatMessageWidget *messageW = new ChatMessageWidget(ui->listWidget->parentWidget());
            QListWidgetItem *item = new QListWidgetItem(ui->listWidget);
            dealMessage(messageW, item, msg, time, ChatMessageWidget::User_Me, image);
        }
        else
        {
            qDebug() << "notSendig";
            dealMessageTime(time);
            ChatMessageWidget *messageW = new ChatMessageWidget(ui->listWidget->parentWidget());
            QListWidgetItem *item = new QListWidgetItem(ui->listWidget);
            dealMessage(messageW, item, msg, time, ChatMessageWidget::User_Me, image);
            messageW->setTextSuccess();
        }
    }
    else
    {
        qDebug()<<"hahahahaa";
        dealMessageTime(time);
        qDebug()<<"hahahahaa";
        ChatMessageWidget *messageW = new ChatMessageWidget(ui->listWidget->parentWidget());
        qDebug()<<"hahahahaa";
        QListWidgetItem *item = new QListWidgetItem(ui->listWidget);
        qDebug()<<"hahahahaa";
        dealMessage(messageW, item, msg, time, ChatMessageWidget::User_She, image);
        qDebug()<<"hahahahaa";
    }
    ui->listWidget->setCurrentRow(ui->listWidget->count() - 1);
}

void MainWindow::dealMessage(ChatMessageWidget *messageW, QListWidgetItem *item, QString text, QString time, ChatMessageWidget::User_Type type, QImage *image)
{
    messageW->setFixedWidth(ui->listWidget->width() - 25);
    QSize size = messageW->fontRect(text);
    item->setSizeHint(size);
    messageW->setText(text, time, size, type, image);
    ui->listWidget->setItemWidget(item, messageW);
}

void MainWindow::dealMessageTime(QString curMsgTime)
{
    bool isShowTime = false;
    if (ui->listWidget->count() > 0)
    {
        QListWidgetItem *lastItem = ui->listWidget->item(ui->listWidget->count() - 1);
        ChatMessageWidget *messageW = (ChatMessageWidget *)ui->listWidget->itemWidget(lastItem);
        int lastTime = messageW->time().toInt();
        int curTime = curMsgTime.toInt();
        qDebug() << "curTime lastTime:" << curTime - lastTime;
        isShowTime = ((curTime - lastTime) > 60); // 两个消息相差一分钟
                                                  //        isShowTime = true;
    }
    else
    {
        isShowTime = true;
    }
    if (isShowTime)
    {
        ChatMessageWidget *messageTime = new ChatMessageWidget(ui->listWidget->parentWidget());
        QListWidgetItem *itemTime = new QListWidgetItem(ui->listWidget);
        QSize size = QSize(ui->listWidget->width() - 25, 40);
        messageTime->resize(size);
        itemTime->setSizeHint(size);
        qDebug() << curMsgTime;
        messageTime->setText(curMsgTime, curMsgTime, size, ChatMessageWidget::User_Time);
        ui->listWidget->setItemWidget(itemTime, messageTime);
    }
}

void MainWindow::on_pushButton_addfriend_clicked()
{
    add = new searchFriends(udata);
    add->show();
}



void MainWindow::on_pushButton_image_clicked()
{
    QString image_addr = QFileDialog::getOpenFileName(this, tr("打开图片"), "/", "Image Files(*.jpg *.jpeg *.bmp *.png)");
    qDebug()<<image_addr;
    if (image_addr.length() == 0)
    {
        return;
    }

    PictureCut *w = new PictureCut(image_addr.toStdString());
    w->show();
    connect(w, SIGNAL(getSelectedPicture(Ui::headImage)), this, SLOT(submitheadImage(Ui::headImage)));
}

void MainWindow::submitheadImage(Ui::headImage hdimage)
{
    QString image_addr = QString::fromStdString(hdimage.path);
    createSendTask(
        udata, image_addr, [=](FileSock *, const QFileInfo &fileName, const QString &serverFileName)
        {
        std::string data = Encoder_submit_image(udata.toStdString(), serverFileName.toStdString());
        QString packData = QString::fromStdString(data);
        client.sendMessage(packData);
        qDebug()<<fileName.absoluteFilePath();
        user_image = QImage(fileName.absoluteFilePath());
        changeMyIcon(&user_image); },
        "head");
}

void MainWindow::on_pushButton_send_image_clicked()
{
    QString image_addr = QFileDialog::getOpenFileName(this);
    if (image_addr.length() == 0)
    {
        return;
    }
    std::vector<std::string> userslist; // userList需要从好友栏导入的，这里固定的占个位置
    for(auto v:chattingInfo.chatFriend)
        userslist.push_back(v.userName);
    sendChatImage(image_addr, userslist, [=]()
    {});
}
void MainWindow::on_biaoqingButton_clicked()
{
    if (!(ui->biaoqingFrame->visibleRegion().isNull()))
    {
        ui->biaoqingFrame->setVisible(false);
    }
    ui->biaoqingFrame->setVisible(true);
}
void MainWindow::on_bButton1_clicked()
{
    sendChatImage(":/assets/微笑.jpg", vector<string>{chattingInfo.chatFriend[0].userName}, [=]()
                  { ui->biaoqingFrame->setVisible(false); });
}
void MainWindow::on_bButton2_clicked()
{
    sendChatImage(":/assets/好！.jpg", vector<string>{chattingInfo.chatFriend[0].userName}, [=]()
                  { ui->biaoqingFrame->setVisible(false); });
}
void MainWindow::on_bButton3_clicked()
{
    sendChatImage(":/assets/大哭！.jpg", vector<string>{chattingInfo.chatFriend[0].userName}, [=]()
                  { ui->biaoqingFrame->setVisible(false); });
}
void MainWindow::on_bButton4_clicked()
{
    sendChatImage(":/assets/冷笑！.jpg", vector<string>{chattingInfo.chatFriend[0].userName}, [=]()
                  { ui->biaoqingFrame->setVisible(false); });
}
void MainWindow::on_bButton5_clicked()
{
    sendChatImage(":/assets/装傻.jpeg", vector<string>{chattingInfo.chatFriend[0].userName}, [=]()
                  { ui->biaoqingFrame->setVisible(false); });
}
void MainWindow::on_bButton6_clicked()
{
    sendChatImage(":/assets/点赞.jpeg", vector<string>{chattingInfo.chatFriend[0].userName}, [=]()
                  { ui->biaoqingFrame->setVisible(false); });
}
void MainWindow::on_bButton7_clicked()
{
    sendChatImage(":/assets/渴望.jpeg", vector<string>{chattingInfo.chatFriend[0].userName}, [=]()
                  { ui->biaoqingFrame->setVisible(false); });
}
void MainWindow::on_bButton8_clicked()
{
    sendChatImage(":/assets/求求你了.jpeg", vector<string>{chattingInfo.chatFriend[0].userName}, [=]()
                  { ui->biaoqingFrame->setVisible(false); });
}
void MainWindow::on_bButton9_clicked()
{
    sendChatImage(":/assets/aaa.png", vector<string>{chattingInfo.chatFriend[0].userName}, [=]()
                  { ui->biaoqingFrame->setVisible(false); });
}

void MainWindow::initConnection()
{
    connect(friendList, SIGNAL(openChatroom(QVariant,int)), this, SLOT(startChatting(QVariant,int)));
    connect(friendList, SIGNAL(delFriend(QVariant)), this, SLOT(deleteFriend(QVariant)));
}
void MainWindow::startChatting(QVariant variant,int a)
{
    ui->cover_label->setVisible(false);
    qDebug()<<"jinlaile";
    clearAllMessage();
    qDebug()<<a;
    isgroup=a;
    if(!a)
    {
        Ui::User chatFriend = variant.value<Ui::User>();
        qDebug() << "here";
        qDebug() << QString(chatFriend.userName.c_str());
        ui->chatName->setText("   " + QString(chatFriend.userName.c_str()));
        chattingInfo.chatFriend.clear();
        chattingInfo.chatFriend.push_back(chatFriend);

        chattingInfo.record = &msgRcd[QString::fromStdString(chatFriend.userName)];
        qDebug() << msgRcd[QString::fromStdString(chatFriend.userName)].size();
        for (MessageInfo rec : *chattingInfo.record)
        {
            bool isyou = (rec.Sender == udata);
            QImage *img = isyou?&user_image:&chattingInfo.chatFriend[0].image;
            if(rec.type == MessageInfo::PLAIN_TXT)
            {
                pushMessageIntoChatWindow(isyou, rec.msg, rec.time, img);
            }
            else if(rec.type == MessageInfo::IMAGE)
            {
                pushImageIntoChatWindow(isyou, rec.img, rec.time, img);
            }
            else if(rec.type == MessageInfo::FILE)
            {
                pushFileIntoChatWindow(isyou, rec.filePath, rec.time, img);
            }
        }
    }
    else
    {
        Ui::Group group = variant.value<Ui::Group>();
        qDebug() << "here";
        qDebug() << QString::number(group.groupid);
        qDebug()<<"hahaha";
        groupid = group.groupid;
        ui->chatName->setText("   " + QString::number(group.groupid));
        chattingInfo.chatFriend.clear();
        for(std::string name:group.member)
        {
            if(QString::fromStdString(name) == udata) continue;
            qDebug()<<QString::fromStdString(name)+" hahaha";
            QTreeWidgetItem* item = friendList->friends[name];
            qDebug()<<QString::fromStdString(name)+" hahaha";
            Ui::User chatFriend = item->data(0,friendList->UserInfo).value<Ui::User>();

            chattingInfo.chatFriend.push_back(chatFriend);
        }

    }

}

void MainWindow::deleteFriend(QVariant variant)
{
    qDebug() << "in delete friend function";
    Ui::User friendToDel = variant.value<Ui::User>();
    qDebug() << "del friend: " << friendToDel.userName.c_str();
    // 返回服务器
    // friendToDel.userName: 删除的好友的名字
    std::string data = Encoder_deletefriends(udata.toStdString(), friendToDel.userName);
    QString packData = QString::fromStdString(data);
    client.sendMessage(packData);
    if (chattingInfo.chatFriend[0].userName == friendToDel.userName)
    {
        ui->cover_label->setVisible(true);
    }
}
//以下是拖动窗口的代码
void MainWindow::mousePressEvent(QMouseEvent *ev)
{
    if (ev->button() == Qt::LeftButton)
    {
        //求坐标差值
        //当前点击坐标-窗口左上角坐标
        p = ev->globalPos() - this->frameGeometry().topLeft();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *ev)
{
    if (ev->buttons() & Qt::LeftButton && p.x() != 0 && p.y() != 0)
    {
        //移到左上角
        move(ev->globalPos() - p);
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *ev)
{
    p.setX(0);
    p.setY(0);
}

/*
 * pushMessageIntoChatWindow函数说明
 * bool type：发给别人消息，右侧气泡==true；接收到消息，左侧气泡==false。
 * QImage msg：发送/接收的Qimage。
 * QString time：消息时间。
 *      参考：time = QString::number(QDateTime::currentDateTime().toTime_t());
 * bool isSending = false：是否正在发送，大概率用不到所以先设置成false
 *      仅发送消息有效，接收消息时这里可以随便赋值。
 * QImage* image = NULL:头像指针，默认为NULL，则显示默认头像。
 *
 */
void MainWindow::pushImageIntoChatWindow(bool type, QImage msg, QString time, QImage *image, bool isSending)
{
    if (type)
    {
        qDebug() << "me";
        if (isSending)
        {
            qDebug() << "sending";
            dealMessageTime(time);
            ChatMessageWidget *messageW = new ChatMessageWidget(ui->listWidget->parentWidget());
            QListWidgetItem *item = new QListWidgetItem(ui->listWidget);
            dealImage(messageW, item, msg, time, ChatMessageWidget::Image_Me, image);
        }
        else
        {
            qDebug() << "not sending";
            dealMessageTime(time);
            ChatMessageWidget *messageW = new ChatMessageWidget(ui->listWidget->parentWidget());
            QListWidgetItem *item = new QListWidgetItem(ui->listWidget);
            dealImage(messageW, item, msg, time, ChatMessageWidget::Image_Me, image);
            messageW->setTextSuccess();
        }
    }
    else
    {
        dealMessageTime(time);
        ChatMessageWidget *messageW = new ChatMessageWidget(ui->listWidget->parentWidget());
        QListWidgetItem *item = new QListWidgetItem(ui->listWidget);
        dealImage(messageW, item, msg, time, ChatMessageWidget::Image_She, image);
    }
    ui->listWidget->setCurrentRow(ui->listWidget->count() - 1);
}

void MainWindow::dealImage(ChatMessageWidget *messageW, QListWidgetItem *item, QImage img, QString time, ChatMessageWidget::User_Type type, QImage *image)
{
    messageW->setFixedWidth(ui->listWidget->width() - 25);
    int w = img.width(), h = img.height();
    QSize size = messageW->fontRect(QString("---------------\n---------------\n---------------\n---------------\n---------------\n---------------\n-"));
    qDebug() << size.width() << "...." << size.height();
    //    QSize size(300,150);
    item->setSizeHint(size);
    messageW->setImage(img, time, type, image);
    ui->listWidget->setItemWidget(item, messageW);
}
void MainWindow::changeMyIcon(QImage *uimg)
{
    if (uimg->isNull())
    {
        uimg = new QImage(":/assets/defaultHead.png");
    }
    QIcon qicon(QPixmap::fromImage(*uimg));
    ui->pushButton_image->setIcon(qicon);
}
void MainWindow::MoveFps() //窗口抖动特效
{
    int befor_x = this->x();
    int befor_y = this->y();
    int move_x = 2;
    int move_y = 3;

    for (int i = 0; i < 10; i++)
    {
        this->move(befor_x + move_x, befor_y);
        Sleep(50);
        this->move(befor_x + move_x, befor_y + move_y);
        Sleep(50);
        this->move(befor_x, befor_y + move_y);
        Sleep(50);
        this->move(befor_x, befor_y);
        Sleep(50);
    }
}

void MainWindow::sendChatImage(const QString &imgFile, std::vector<string> userList, std::function<void()> onSuccess)
{
    createSendTask(
        udata, imgFile, [=](FileSock *skt, const QFileInfo &fileName, const QString &serverFileName)
        {
//        std::vector<std::string> userslist; // userList需要从好友栏导入的，这里固定的占个位置
//        userslist.push_back(chattingInfo.chatFriend[0].userName);

        std::string data = Encoder_chatfile(serverFileName.toStdString(), udata.toStdString(), userList);
        QString packData = QString::fromStdString(data);
        client.sendMessage(packData);
        QImage chatimage(fileName.absoluteFilePath());
        pushImageIntoChatWindow(true, chatimage, QString::number(QDateTime::currentDateTime().toTime_t()), &user_image);
        onSuccess(); });
}


void MainWindow::pushFileIntoChatWindow(bool type,QString filePath, QString time, QImage *image, bool isSending)
{
    QString msg = filePath.right(filePath.lastIndexOf("/")+1);

    qDebug() << "addFile" << msg << time << ui->listWidget->count();
    if (type)
    {
        if (isSending)
        {
            dealMessageTime(time);
            ChatMessageWidget *messageW = new ChatMessageWidget(ui->listWidget->parentWidget());
            QListWidgetItem *item = new QListWidgetItem(ui->listWidget);
            dealFile(messageW, item,filePath ,msg, time, ChatMessageWidget::File_Me, image);
        }
        else
        {
            dealMessageTime(time);
            ChatMessageWidget *messageW = new ChatMessageWidget(ui->listWidget->parentWidget());
            QListWidgetItem *item = new QListWidgetItem(ui->listWidget);
            dealFile(messageW, item,filePath, msg, time, ChatMessageWidget::File_Me, image);
            messageW->setTextSuccess();
        }
    }
    else
    {
        dealMessageTime(time);
        ChatMessageWidget *messageW = new ChatMessageWidget(ui->listWidget->parentWidget());
        QListWidgetItem *item = new QListWidgetItem(ui->listWidget);
        dealFile(messageW, item,filePath, msg, time, ChatMessageWidget::User_She, image);
    }
    ui->listWidget->setCurrentRow(ui->listWidget->count() - 1);
}

void MainWindow::dealFile(ChatMessageWidget *messageW, QListWidgetItem *item,QString filePath, QString text, QString time, ChatMessageWidget::User_Type type, QImage *image)
{
    text = QString("【📂双击打开文件】")+text;
    messageW->setFixedWidth(ui->listWidget->width() - 25);
    QSize size = messageW->fontRect(text);
    item->setSizeHint(size);
    messageW->setFile(filePath,text, time, size, type, image);
    ui->listWidget->setItemWidget(item, messageW);
}

void MainWindow::changeIcon(QString username, QImage icon)
{
    friendList->changeIcon(username, icon);
}

void MainWindow::on_pushButton_3_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, "选择文件");
    qDebug()<<filename;
    if(filename.length() ==0)
        return;
    QFileInfo fileinfo(filename);
    createSendTask(udata, filename, [=](FileSock *, const QFileInfo &, const QString &){
        vector<string> userList;
        for(auto v:chattingInfo.chatFriend)
            userList.push_back(v.userName);
        string packData = Encoder_chatfile(filename.toStdString(), udata.toStdString(),userList);
        client.sendMessage(QString::fromStdString(packData));
        pushFileIntoChatWindow(true, filename, QString::number(QDateTime::currentDateTime().toTime_t()), &user_image);
        msgRcd[QString::fromStdString(chattingInfo.chatFriend[0].userName)].push_back(MessageInfo(udata, filename, QString::number(QDateTime::currentDateTime().toTime_t()), MessageInfo::FILE));
    });
}

void MainWindow::on_pushButton_addgroup_clicked()
{
    group *g = new group(friendList, udata);
    g->show();
}
