#include "Tools.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <iostream>

using namespace std;

int StrParser::toInt(string str)
{
    stringstream ss(str);
    int a;
    ss >> a;
    return a;
}

Json::Value makeCmd(std::string type, Json::Value cmd)
{
    Json::Value response;
    response["type"] = type;
    response["info"] = cmd;
    return response;
}

int sendStr(fd_t client, const char *str, uint len)
{
    char sendmsg[5000] = {0};
    uInt2Bytes(len, sendmsg);
    memcpy(sendmsg + 4, str, len);
    return send(client, sendmsg, len + 4, 0) - 4;
}

void sendJson(fd_t client, Json::Value json)
{
    string jsonStr = encodeJson(json);
    cout << jsonStr << endl;
    cout << "sending bytes: " << jsonStr.length() << endl;
    char sendmsg[4096];
    uInt2Bytes(jsonStr.length(), sendmsg);
    strcpy(sendmsg + 4, jsonStr.c_str());
    // cout << "sending actually bytes: " << (sendmsg + jsonStr).length() << endl;
    send(client, sendmsg, jsonStr.length() + 4, 0);
}

uint bytes2uInt(const char *bytes)
{
    uint b0 = (uint)bytes[0] & 0xFFu;
    uint b1 = (uint)bytes[1] & 0xFFu;
    uint b2 = (uint)bytes[2] & 0xFFu;
    uint b3 = (uint)bytes[3] & 0xFFu;
    return (b0 << 24) | (b1 << 16) | (b2 << 8) | b3;
}

void uInt2Bytes(uint _n, char *buf)
{
    buf[0] = 0xFFu & (_n >> 24);
    buf[1] = 0xFFu & (_n >> 16);
    buf[2] = 0xFFu & (_n >> 8);
    buf[3] = 0xFFu & (_n);
}