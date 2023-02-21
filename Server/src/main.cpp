#include "Server.h"
#include "sql++.h"
int main()
{
    Sql::singleton().open();
    Server &server = Server::singleton(htonl(INADDR_ANY), 8888, 9999);
    server.connect();
    server.run();
    return 0;
}