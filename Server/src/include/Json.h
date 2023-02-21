/**
 * 将json库做了个封装，方便使用
 */
#ifndef __MJSON_H__
#define __MJSON_H__

#include <json/json.h>
#include <string>

extern Json::Reader j_reader;
extern Json::FastWriter j_fwriter;

Json::Value decodeJson(std::string jsonStr);
std::string encodeJson(Json::Value jsonData);

#endif
