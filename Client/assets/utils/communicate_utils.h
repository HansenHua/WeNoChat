#ifndef COMMUNICATE_UTILS_H
#define COMMUNICATE_UTILS_H
#include <QWidget>
#include <QDebug>
#include <QMessageBox>
#include <QTcpSocket>
#include "json/json.h"
#include <cstring>
#include <string>
#include <iostream>
#include <vector>
#include "UI/friendlist.h"
std::string Encoder(std::string type, Json::Value info); //将发送的信息按json文件的字符串格式编码，输出string
//add_group_chat
std::string Encoder_add_group_chat(std::string groupname, std::vector<std::string> user);
int Decoder_add_group_chat(Json::Value packdata, std::vector<std::string> &username, int &groupid, bool &state, std::string &groupname);
//chat_group
std::string Encoder_chat_group(std::string username, int groupid, std::string info, std::string time);
int Decoder_chat_group(Json::Value packdata, std::string &username, int &groupid, std::string &info, std::string &time);
// login
std::string Encoder_login(std::string username, std::string pwd);                               // login编码
int Decoder_login(Json::Value packdata, std::string &username, std::string &info, bool &state); // login解码

// regist
std::string Encoder_regist(std::string username, std::string password, std::string phonenum, int secureQue, std::string secureAns);
int Decoder_regist(Json::Value packdata, std::string &username, std::string &info, bool &state); // regist解码

// chat
std::string Encoder_chat(std::string username, std::string info, std::string time, std::vector<std::string> userList);
int Decoder_chat(Json::Value packdata, std::string &sender_username, std::string &info, std::string &time);

// addfriends加好友
std::string Encoder_addfriends(std::string username, std::string friend_username);
int Decoder_addfriends(Json::Value packdata, std::string &username, bool &state, std::string &friend_image);

// askfriendsList 请求好友列表
std::string Encoder_askfriendsList(std::string username);
int Decoder_askfriendsList(Json::Value packdata, std::string &username, std::vector<std::pair<std::string, std::string>> &user_info_List, std::string &user_image);

// deletefriends 删除好友
std::string Encoder_deletefriends(std::string username, std::string friend_username);
int Decoder_deletefriends(Json::Value packdata, std::string username, std::string friend_username);

// inform_chatfile 发图片前先通知服务器
std::string Encoder_inform_chatfile(std::string filename, std::string username, int size);
int Decoder_ready_chatfile(Json::Value packdata, bool &state);

// chatfile 聊天发送图片
std::string Encoder_chatfile(std::string filename, std::string username, std::vector<std::string> userList);
int Decoder_chatfile(Json::Value packdata, std::string &filename, std::string &sender_name, std::string &time);

// submit_image上传个人头像
std::string Encoder_submit_image(std::string username, std::string image);
int Decoder_submit_image(Json::Value packdata, std::string &username, bool &state);

// findpWord1
std::string Encoder_findpWord1(std::string username, std::string phonenum);
int Decoder_findpWord1(Json::Value packdata, std::string &username, bool &state, int &secureQue, std::string &info);

// findpWord2
std::string Encoder_findpWord2(std::string username, std::string secureAns);
int Decoder_findpWord2(Json::Value packdata, std::string &username, bool &state);

// findpWord3
std::string Encoder_findpWord3(std::string username, std::string newpWord);
int Decoder_findpWord3(Json::Value packdata, std::string &username, bool &state);
#endif // COMMUNICATE_UTILS_H
