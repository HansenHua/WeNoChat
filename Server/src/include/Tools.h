/**
 * 工具类
 */

#ifndef __TOOLS_H__
#define __TOOLS_H__

#include <string>
#include <sstream>
#include <set>
#include "Json.h"
#include "Constants.h"

namespace StrParser
{
    /**
     * @brief 转换字符串为int
     */
    int toInt(std::string);
}

// 打包发回客户端的信息
Json::Value makeCmd(std::string type, Json::Value cmd);

// 向客户端发送json消息
void sendJson(fd_t client, Json::Value json);

int sendStr(fd_t client, const char *str, uint len);

uint32_t bytes2uInt(const char *bytes);

void uInt2Bytes(uint32_t _n, char *buf);
#endif