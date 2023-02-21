/**
 * 服务端
 * 用来处理和客户端之间的通信
 */

#ifndef __SERVER_H__
#define __SERVER_H__

#include <arpa/inet.h>
#include <unordered_set>
#include <sys/epoll.h>

#include "Constants.h"
#include "CmdHandler.h"

class Server
{
public:
    /**
     * 获取Server实例
     * Server &server = Server::singleton();
     */
    static Server &singleton(uint32_t addr = 0, uint16_t port = 0, uint16_t filePort = 0);

    //连接服务端
    void connect();

    void run();
    fd_t getFdByName(std::string username);
    void addClient(std::string username, fd_t __fd);

    int sendFile(fd_t fileClient, std::string filepath);
    void recvFile();

private:
    Server(bool inited, uint32_t addr = 0, uint16_t port = 0, uint16_t filePort = 0);
    Server(const Server &) = delete;
    Server &operator=(const Server &) = delete;

    uint32_t addr;
    uint16_t port;
    uint16_t filePort;
    fd_t fileSock;
    fd_t sock;
    fd_t epoll_fd;
    std::unordered_set<fd_t> client_fds;
    std::unordered_set<fd_t> fileClient_fds;
    std::unordered_map<std::string, fd_t> clients;
    std::unordered_map<fd_t, std::string> __clients;

    std::unordered_map<fd_t, WriteFileTask> fileSendTasks;

    CmdHandler &handler;
};

#endif