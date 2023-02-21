#include "Json.h"
#include <iostream>

using namespace std;

Json::Reader j_reader;
Json::FastWriter j_fwriter;

Json::Value decodeJson(std::string jsonStr)
{
    Json::Value value;
    if (!j_reader.parse(jsonStr, value))
        cout << "json parse error" << endl;
    return value;
}
std::string encodeJson(Json::Value jsonData)
{
    return j_fwriter.write(jsonData);
}