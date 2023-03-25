#if !defined(SERVER_HPP)
#define SERVER_HPP

#include <iostream>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>
#include <map>
#include "../Parse/Config.hpp"
#include "../Parse/requestParse.hpp"

struct Client
{
    socklen_t addressLenght;
    struct sockaddr_storage clientAddress;
    int socket;
    int received;
    int remaining;
    int connectionClosed;
    const std::string getClientAddress();
};

class Server
{
private:
    int                         _serverSocket;
    std::map<int, Client>       _clients;
    fd_set                      _readyToReadFrom;
    Config                      _configFile;

    std::string getRequestBuffer(Client &);
    void checkCientCases();
    void getReadableClient();
    void acceptConnection();
    void serveContent();
    void initServerSocket(const char *host, const char *port);
public:
    Server(std::string file);
    ~Server();
};

#endif // SERVER_HPP
