#ifndef FRIENDLIST_H
#define FRIENDLIST_H

#include <QWidget>
#include <string>
#include <QTreeWidget>
#include <QImage>
#include<map>
#include<string>
namespace Ui
{
    class FriendList;
    struct User : public QObjectUserData
    {
        std::string userName;
        QImage image;
//        std::string imageStr;
    };
    struct Group : public QObjectUserData
    {
        int groupid;
        std::vector<std::string> member;
    };

    class FriendList : public QWidget
    {
        Q_OBJECT

    public:
        QTreeWidget *friendlist;
        enum ItemType
        {
            isGroup = Qt::UserRole,
            UserInfo = Qt::UserRole + 1
        };
        explicit FriendList(QTreeWidget *widget, QWidget *parent=0);
        explicit FriendList(QTreeWidget *widget, std::vector<Ui::User> userInfoList, QWidget *parent = 0);
        ~FriendList();
        void addFriendInfo(QTreeWidgetItem *group, std::vector<Ui::User> friendInfoList);
        void addGroupInfo( Ui::Group groupInfoList);

        void addGroup(std::string name);
        void insertToGroup(std::string, std::vector<Ui::User> friendInfoList);
        void deleteGroup(std::string name);
        //TODO: delete friend;
        void deleteFriend(std::string groupName, std::string friendName);
        std::map<std::string, QTreeWidgetItem *> getGroups();
        std::map<std::string, QTreeWidgetItem *> getFriends();
        void changeIcon(QString username, QImage icon);
        std::map<std::string, QTreeWidgetItem *> groups;
        std::map<std::string, QTreeWidgetItem *> friends;
        void initFriendList(std::vector<Ui::User> userInfoList);
    private:

        void initConnection();



    signals:
        void openChatroom(QVariant variant, int a);
        void delFriend(QVariant variant);

    private slots:
        void showFriendListMenu(QPoint pos);

        // private slots:
        //     void groupExpand(QTreeWidgetItem* item);
    };
}

Q_DECLARE_METATYPE(Ui::User)
Q_DECLARE_METATYPE(Ui::Group)
#endif // FRIENDLIST_H
