
#include "../inc.hpp"
#include "Server.hpp"
#include "../Parse/requestParse.hpp"
#include "../Response/Response.hpp"

#define MAX_REQUEST_SIZE 4096

void error(const char *s)
{
    perror(s);
    exit(1);
}

void Server::initServerSocket(const char *host, const char *port)
{
    struct addrinfo hints, *data;
    memset(&hints, 0, sizeof(hints));
    hints.ai_flags = AI_PASSIVE;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_family = AF_INET;
    int statusCode = getaddrinfo(host, port, &hints, &data);
    if (statusCode)
    {
        std::cerr << gai_strerror(statusCode) << std::endl;
        std::exit(1);
    }
    std::cout << "Creating the server socket\n";
    _serverSocket = socket(data->ai_family, data->ai_socktype, data->ai_protocol);
    if (_serverSocket == -1)
        error("socket()");
    std::cout << "Binding the server socket to local address\n";
    if (bind(_serverSocket, data->ai_addr, data->ai_addrlen))
        error("bind()");
    std::cout << "Listening for requests\n";
    if (listen(_serverSocket, 10))
        error("listen()");
    freeaddrinfo(data);
}

void Server::getReadableClient()
{
    FD_ZERO(&_readyToReadFrom);
    FD_SET(_serverSocket, &_readyToReadFrom);
    int maxSocket = _serverSocket;
    std::map<int, Client>::iterator it = _clients.begin();
    while (it != _clients.end())
    {
        FD_SET(it->first, &_readyToReadFrom);
        if (it->first > maxSocket)
            maxSocket = it->first;
        ++it;
    }

    if (select(maxSocket + 1, &_readyToReadFrom, 0, 0, 0) == -1)
        error("select()");
}

Server::~Server()
{
    _clients.clear();
}

const std::string Client::getClientAddress()
{
    char buff[100];
    getnameinfo((struct sockaddr *)&this->clientAddress,
                this->addressLenght, buff, sizeof buff, 0, 0,
                NI_NUMERICHOST);
    const std::string save(buff);
    return save;
}

void Server::acceptConnection()
{
    if (FD_ISSET(_serverSocket, &_readyToReadFrom))
    {
        Client _newClient;
        _newClient.addressLenght = sizeof _newClient.clientAddress;
        _newClient.socket = accept(_serverSocket,
                                   (struct sockaddr *)&_newClient.clientAddress,
                                   &_newClient.addressLenght);
        if (_newClient.socket == -1)
            error("accept()");
        _clients.insert(std::make_pair(_newClient.socket,
                                       _newClient));
        std::cout << "New connection from: "
                  << _newClient.getClientAddress() << "\n";
    }
}


#define MAX_CHUNK_SIZE 1024

void Server::serveContent()
{
    signal(SIGPIPE, SIG_IGN);
    std::map<int, Client>::iterator it = _clients.begin();
    while (it != _clients.end())
    {
        if (FD_ISSET(it->first, &_readyToReadFrom))
        {
            int r;
            char buff[MAX_REQUEST_SIZE];
            memset(buff, '\0', sizeof buff);
            r = recv(it->first, buff, sizeof buff, 0);
            if (r < 1)
            {
                close(it->first);
                _clients.erase(it++);
                continue;
            }
            else
            {
                requestParse request(buff);
                Response response(_configFile, request);
                it->second.remaining = response._response.size();
                it->second.received = 0;
                while (it->second.received < (int)response._response.size())
                {
                    int chunk = std::min(it->second.remaining, MAX_CHUNK_SIZE);
                    const char *chunkedStr = response._response.c_str() + it->second.received;
                    int ret = send(it->first, chunkedStr, chunk, 0);
                    if (ret == -1)
                        break;
                    it->second.remaining -= ret;
                    it->second.received += ret;
                }
                close(it->first);
                _clients.erase(it++);
                continue;
            }
        }
        else
            it++;
    }
}

Server::Server(std::string file) : _configFile(file)
{
    initServerSocket(NULL, "800");
    while (1)
    {
        getReadableClient();
        acceptConnection();
        serveContent();
    }
    close(_serverSocket);
}

int main(int ac, char **av)
{
    if (ac == 2)
        Server server(av[1]);
}
