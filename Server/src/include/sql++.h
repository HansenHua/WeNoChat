/**
 * 将sqlite3封装后的类。
 */

#ifndef __SQLXX_H__
#define __SQLXX_H__

#include <sqlite3.h>
#include <vector>
#include <string>

#include "Record.h"
#include "Tools.h"

class Sql
{
public:
    static Sql &singleton();

    void open();
    std::vector<UserRecord> findUserByName(std::string userName);
    std::vector<UserRecord> findUserByPhone(std::string phone);
    int insertUser(UserRecord rec);
    int updateUser(std::string username, std::string column, std::string value);
    std::vector<UserRecord> findFriends(std::string userName);
    int insertFriends(std::string user, std::string friend_user);
    int deleteFriends(std::string user, std::string friend_user);
    std::vector<std::string> getGroupMembers(std::string groupid);
    int getGroupCounts();
    int insertGroups(std::string username, std::string groupid);
    int insertGroupInfo(std::string groupid, std::string groupname);

private:
    Sql() {}
    Sql(const Sql &) = delete;
    Sql &operator=(const Sql &) = delete;
    sqlite3 *mySqlite;
};

#endif