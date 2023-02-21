/**
 * 数据表类
 * 存的是各个字段。
 */

#ifndef __TABLES_H__
#define __TABLES_H__

#include <string>

struct UserRecord
{
    std::string username;
    std::string password;
    std::string phonenum;
    int secureQue;
    std::string secureAns;
    std::string headfile;
};
/**
 * TODO: 定义每个数据表对应的结构体
 */

#endif