#if !defined(SERVER_HPP)
#define SERVER_HPP

#include "../inc.hpp"
#include "../Parse/Config.hpp"
#include "../Parse/requestParse.hpp"

struct Client
{
    socklen_t addressLenght;
    struct sockaddr_storage clientAddress;
    int socket;
    int received;
    int remaining;
    const std::string getClientAddress();

    Client();
};

class Server
{
private:
    int                         _serverSocket;
    std::vector<Client>         _clients;
    fd_set                      _readyToReadFrom;
    // fd_set                      _readyToWriteTo;
    Config                      _configFile;
    
    requestParse    getRequest(const Client &_client);
    std::string     getRequestBuffer(Client &);
    void            checkCientCases();
    void            getReadableClient();
    void            acceptConnection();
    void            serveContent();
    void            initServerSocket(const char *host, const char *port);
public:
    Server(std::string file);
    ~Server();
};

#endif // SERVER_HPP
