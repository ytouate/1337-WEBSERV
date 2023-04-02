
#include "../inc.hpp"
#include "Server.hpp"
#include "../Parse/requestParse.hpp"
#include "../Response/Response.hpp"
#include "../Parse/serverParse.hpp"
#define MAX_REQUEST_SIZE 4096
#define MAX_CHUNK_SIZE 25000
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

    // setting the socket to be non blocking
    fcntl(_serverSocket, F_SETFL, O_NONBLOCK);

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
    // FD_ZERO(&_readyToWriteTo);
    FD_SET(_serverSocket, &_readyToReadFrom);
    // FD_SET(_serverSocket, &_readyToWriteTo);
    int maxSocket = _serverSocket;
    std::vector<Client>::iterator it = _clients.begin();
    while (it != _clients.end())
    {

        FD_SET(it->socket, &_readyToReadFrom);
        // FD_SET(it->socket, &_readyToWriteTo);
        if (it->socket > maxSocket)
            maxSocket = it->socket;
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
        _clients.push_back(_newClient);
        std::cout << "New connection from: "
                  << _newClient.getClientAddress() << "\n";
    }
}


requestParse Server::getRequest(const Client &_client)
{
    int bytesRead, bytesLeft;
    char buff[MAX_REQUEST_SIZE];
    while ((bytesRead = recv(_client.socket, buff, MAX_REQUEST_SIZE, 0)) > 0)
    {
        buff[bytesRead] = '\0';
        if (bytesRead == 2 && buff[0] == '\r' && buff[1] == '\n')
            break;
    }

    requestParse request(buff);
    bytesLeft = atoi(request.data["content-length"].c_str());
    memset(buff, 0, sizeof buff);
    while (bytesLeft > 0)
    {
        bytesRead = recv(_client.socket, buff, std::min(bytesLeft, MAX_REQUEST_SIZE), 0);
        if (bytesRead <= 0)
            break;
        bytesLeft -= bytesRead;
        request.body.content += buff;
        memset(buff, 0, sizeof buff);
    }
    request.body.setUp();
    request.data["body"] = buff;
    return request;
}

void Server::serveContent()
{
    signal(SIGPIPE, SIG_IGN);
    std::vector<Client>::iterator it = _clients.begin();
    while (it != _clients.end())
    {
        if (FD_ISSET(it->socket, &_readyToReadFrom))
        {
            requestParse request = getRequest(*it);
            Response response(_configFile, request);
            it->remaining = response._response.size();
            it->received = 0;
            while (it->received < (int)response._response.size())
            {
                int chunk = std::min(it->remaining, MAX_CHUNK_SIZE);
                const char *chunkedStr = response._response.c_str() + it->received;
                int ret = send(it->socket, chunkedStr, chunk, 0);
                if (ret == -1)
                {
                    close(it->socket);
                    break;
                    // it = _clients.erase(it);
                    // continue;
                    // // how do i delete the iterator from the vector without affecting the loop
                    // break;
                }
                it->remaining -= ret;
                it->received += ret;
            }
            close(it->socket);
            it = _clients.erase(it);
        }
        else
        {
            it++;
        }
    }
}

Server::Server(std::string file) : _configFile(file)
{
    const char *port = "8080";
    initServerSocket(NULL, port);
    std::cout << "http://localhost:" << port << std::endl;
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
