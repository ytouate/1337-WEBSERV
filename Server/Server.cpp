
#include "../inc.hpp"
#include "Server.hpp"
#include "../Parse/requestParse.hpp"
#include "../Response/Response.hpp"
#include "../Parse/serverParse.hpp"

#define MAX_REQUEST_SIZE 4096
#define MAX_CHUNK_SIZE 200

static void error(const char *s)
{
    perror(s);
    exit(1);
}

void Server::initServerSocket(const char *port)
{
    struct addrinfo hints, *data;
    memset(&hints, 0, sizeof(hints));
    hints.ai_flags = AI_PASSIVE;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_family = AF_INET;
    int statusCode = getaddrinfo(NULL, port, &hints, &data);
    if (statusCode)
    {
        std::cerr << gai_strerror(statusCode) << std::endl;
        std::exit(1);
    }

    std::cout << "Creating the server socket\n";
    _serverSocket = socket(data->ai_family, data->ai_socktype, data->ai_protocol);
    if (_serverSocket == -1)
        error("socket()");

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
    FD_SET(_serverSocket, &_readyToReadFrom);
    int maxSocket = _serverSocket;
    std::vector<Client>::iterator it = _clients.begin();
    while (it != _clients.end())
    {
        FD_SET(it->socket, &_readyToReadFrom);
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
        // std::cout << "New connection from: "
        //           << _newClient.getClientAddress() << "\n";
    }
}

requestParse Server::getRequest(const Client &_client)
{
    fcntl(_client.socket, F_SETFL, O_NONBLOCK);
    int bytesRead, bytesLeft;
    char buff[MAX_REQUEST_SIZE];
    std::string header;
    while ((bytesRead = recv(_client.socket, buff, MAX_REQUEST_SIZE, 0)) > 0)
    {
        header.append(std::string(buff, bytesRead));
        if (bytesRead == 2 && buff[0] == '\r' && buff[1] == '\n')
            break;
        memset(buff, 0, sizeof buff);
    }
    requestParse request(header);
    bytesLeft = atoi(request.data["content-length"].c_str());
    if (bytesLeft == 0)
        return request;
    memset(buff, 0, sizeof buff);
    while (bytesLeft > 0)
    {
        bytesRead = recv(_client.socket, buff, std::min(bytesLeft, MAX_REQUEST_SIZE), 0);
        if (bytesRead <= 0)
            break;
        bytesLeft -= bytesRead;
        request.body.content.append(std::string(buff, bytesRead));
        memset(buff, 0, sizeof buff);
    }
    int fd = open("9bel", O_RDWR | O_CREAT, 0644);
    int fd2 = open("ba3d", O_RDWR | O_CREAT, 0644);
    write(fd, request.body.content.c_str(), request.body.content.size());
    request.body.setUp();
    write(fd2, request.body.content.c_str(), request.body.content.size());
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
            if (request.data["method"] == "POST")
            {
                if (request.body.contentName.empty())
                {
                    it->_waitingForBody = true;
                    continue;
                }
                else if (it->_waitingForBody)
                    it->_waitingForBody = false;
            }
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
                    perror("send()");
                    close(it->socket);
                    break;
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
    srand(time(NULL));
    int port = (rand() % (65535 - 1024 + 1)) + 1024;

    initServerSocket(std::to_string(port).c_str());
    std::cout << "http://localhost:" << port << std::endl;
    while (1)
    {
        getReadableClient();
        acceptConnection();
        serveContent();
    }

    close(_serverSocket);
}

Client::Client() : received(0), remaining(0), _waitingForBody(false) {}

int main(int ac, char **av)
{
    if (ac == 2)
        Server server(av[1]);
}
