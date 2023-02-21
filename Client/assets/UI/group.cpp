#include "group.h"
#include "ui_group.h"
using namespace std;

group::group(Ui::FriendList *f, QString ud,QWidget *parent) :
    QWidget(parent),
    ud(ud),
    client(DataSock::get()),
    ui(new Ui::group)
{
    ui->setupUi(this);
    friendlist = f;
    map<string, QTreeWidgetItem *> friends = friendlist->getFriends();
    for(map<string, QTreeWidgetItem *>::iterator it = friends.begin(); it!=friends.end(); it++)
    {
        QListWidgetItem *item = new QListWidgetItem();
        QCheckBox *checkbox = new QCheckBox();

        ui->listWidget->addItem(item);
        ui->listWidget->setItemWidget(item,checkbox);
        //TODO:设置好友列表
        checkbox->setText(it->second->text(0));

        connect(checkbox, SIGNAL(stateChanged(int)),this,SLOT(anystateChanged(int)));
    }
}

group::~group()
{
    delete ui;
}

void group::anystateChanged(int __n)
{
    selectedItems.clear();
    map<string, QTreeWidgetItem *> friends = friendlist->getFriends();
    for(int i = 0; i < ui->listWidget->count();i++)
    {
        QListWidgetItem *item = ui->listWidget->item(i);
        QCheckBox *checkbox = static_cast<QCheckBox *>(ui->listWidget->itemWidget(item));
        if(checkbox->isChecked())
        {
            selectedItems.append(i);
        }
    }
}

void group::on_pushButton_clicked()
{
    QString groupname = ui->lineEdit->text();
    qDebug()<<selectedItems;
    std::vector<std::string> user;
    for(int i = 0;i < selectedItems.size();i++)
    {
        user.push_back(friendlist->getGroups()["default"]->child(selectedItems.at(i))->text(0).toStdString());
        //把群聊中好友加进去
    }
    user.push_back(ud.toStdString());
    std::string data = Encoder_add_group_chat(groupname.toStdString(), user);
    QString packData = QString::fromStdString(data);
    client.sendMessage(packData);
    this->close();
}
