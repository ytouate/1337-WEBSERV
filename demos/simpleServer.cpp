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
#include "simpleServer.hpp"
#include "../Parse/requestParse.hpp"
#include "../Response/Response.hpp"

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
    int biggerSocket = _serverSocket;
    std::map<int, Client>::iterator it = _clients.begin();
    while (it != _clients.end())
    {
        FD_SET(it->first, &_readyToReadFrom);
        if (it->first > biggerSocket)
            biggerSocket = it->first;
        ++it;
    }

    if (select(biggerSocket + 1, &_readyToReadFrom, 0, 0, 0) == -1)
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

Server::Server()
{
    initServerSocket(NULL, "8080");
    while (1)
    {
        getReadableClient();
        acceptConnection();
        std::map<int, Client>::iterator it = _clients.begin();
        while (it != _clients.end())
        {
            if (FD_ISSET(it->first, &_readyToReadFrom))
            {
                int r;
                char buff[1000];
                r = recv(it->first, buff, sizeof buff, 0);
                if (r < 1)
                {
                    close(it->first);
                    it = _clients.erase(it);
                    continue;
                }
                std::string response;
                response += "HTTP/1.1 200 OK\r\n";
                response += "Connection: close\r\n";
                response += "Content-Length: 21\r\n";
                response += "Content-Type: text/html\r\n";
                response += "\r\n";
                response += "<h1>Hello world</h1>";
                int nBytes = send(it->first, response.c_str(),
                                  response.size(), 0);
                if (nBytes == -1)
                    error("send()");
                close(it->first);
                it = _clients.erase(it);
                std::cout << nBytes << " byte Sent\n";
            }
            it++;
        }
    }
}

int main()
{
    Server server;
}
