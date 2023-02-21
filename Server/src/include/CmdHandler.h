#ifndef __CMD_HANDLER_H__
#define __CMD_HANDLER_H__

#include <unordered_map>
#include <string>
#include <functional>
#include <sys/socket.h>
#include <sys/types.h>
#include <list>

#include "File.h"
#include "Tools.h"
#include "Constants.h"
#include "Record.h"

#include "Json.h"

class CmdHandler
{
public:
    static CmdHandler &singleton();
    void handle(fd_t client, const char *buf, int _n);

    std::unordered_map<std::string, UserRecord> pWordForgotters;
    std::unordered_map<fd_t, WriteFileTask> fileTasks;
    std::unordered_map<std::string, std::list<std::string>> msgRcds;

private:
    CmdHandler();
    CmdHandler(const CmdHandler &) = delete;
    CmdHandler &operator=(const CmdHandler &) = delete;
    std::unordered_map<std::string, std::function<void(fd_t client, Json::Value cmd)>> __callbacks;
};

namespace __Callbacks
{
    void _login(fd_t confd, Json::Value cmd);
    void _register(fd_t client, Json::Value cmd);
    void _chat(fd_t client, Json::Value cmd);
    void _getFriends(fd_t client, Json::Value cmd);
    void _addFriends(fd_t client, Json::Value cmd);
    void _deleteFriends(fd_t client, Json::Value cmd);
    void _findPword_phone(fd_t client, Json::Value cmd);
    void _findPword_que(fd_t client, Json::Value cmd);
    void _findPword_change(fd_t client, Json::Value cmd);
    void _cancelFindPword(fd_t client, Json::Value cmd);
    void _sendFile(fd_t fileClient, Json::Value cmd);
    void _updateFile(fd_t fileClient, const char *buf, int _n);
    void _rqirFile(fd_t fileClient, Json::Value cmd);
    void _chatFile(fd_t client, Json::Value cmd);
    void _submitImage(fd_t client, Json::Value cmd);
}
#endif // CmdHandler.h