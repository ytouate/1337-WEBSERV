#include "../inc.hpp"
#include "Server.hpp"
#include "../Parse/requestParse.hpp"
#include "../Response/Response.hpp"

#include "../Parse/Config.hpp"

#define MAX_REQUEST_SIZE 4096
#define MAX_CHUNK_SIZE 4096

// Write the last error happened in s method and exit
static void error(const char *s)
{
    perror(s);
    exit(1);
}

/*
    initializing the socket and binding it to local address
    and listening for new connections
*/
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
    int ret = socket(data->ai_family, data->ai_socktype, data->ai_protocol);
    if (ret == -1)
        error("socket()");
    _serverSockets.push_back(ret);
    int optval = 1;
    setsockopt(_serverSockets.back(), SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));
    fcntl(_serverSockets.back(), F_SETFL, O_NONBLOCK);
    if (bind(_serverSockets.back(), data->ai_addr, data->ai_addrlen))
        error("bind()");
    if (listen(_serverSockets.back(), 10))
        error("listen()");
    freeaddrinfo(data);
}

/*
    adds new client to the _readyToWriteTo and _readyToReadFrom fd_sets
    and handles communication with already connected clients using select
*/
void Server::waitForClients()
{
    FD_ZERO(&_readyToReadFrom);
    FD_ZERO(&_readyToWriteTo);
    int maxSocket = _serverSockets.front();
    for (size_t i = 0; i < _serverSockets.size(); ++i)
    {
        FD_SET(_serverSockets[i], &_readyToReadFrom);
        if (_serverSockets[i] > maxSocket)
            maxSocket = _serverSockets[i];
    }
    std::vector<Client>::iterator it = _clients.begin();
    while (it != _clients.end())
    {
        FD_SET(it->socket, &_readyToReadFrom);
        FD_SET(it->socket, &_readyToWriteTo);
        if (it->response.fdIsOpened || it->response._fd != -1)
        {
            FD_SET(it->response._fd, &_readyToWriteTo);
            if (it->response._fd > maxSocket)
                maxSocket = it->response._fd;
        }
        if (it->socket > maxSocket)
            maxSocket = it->socket;
        ++it;
    }
    if (select(maxSocket + 1, &_readyToReadFrom, &_readyToWriteTo, 0, 0) == -1)
        error("select()");
}

/*
    Destructor that clears the _clients vector
*/
Server::~Server()
{
    _clients.clear();
}

/*
    returns the client's ip address in form of a string
*/
const std::string Client::getClientAddress()
{
    char buff[100];
    getnameinfo((struct sockaddr *)&this->clientAddress,
                this->addressLenght, buff, sizeof buff, 0, 0,
                NI_NUMERICHOST);
    const std::string save(buff);
    return save;
}

/*
    checks if the server socket able to be read from
    if so a connection is accepted and a new client is
    added to the _clients vector
*/
void Server::acceptConnection(int serverIndex)
{
    if (FD_ISSET(_serverSockets[serverIndex], &_readyToReadFrom))
    {
        Client _newClient;
        _newClient.addressLenght = sizeof _newClient.clientAddress;
        _newClient.socket = accept(_serverSockets[serverIndex],
                                   (struct sockaddr *)&_newClient.clientAddress,
                                   &_newClient.addressLenght);
        if (_newClient.socket == -1)
            error("accept()");
        _clients.push_back(_newClient);
    }
}

/*
    returns true if the passed string is a hexadecimal number
    representing the chunk line
*/
bool isChunkLine(const std::string &s)
{
    if (s.empty())
        return false;
    if (s.back() != '\r')
    {
        return false;
    }
    for (size_t i = 0; i < s.size(); ++i)
    {
        if (!std::isxdigit(s[i]) && (s[i] != '\r' && s[i] != '\n'))
        {
            return false;
        }
    }
    return true;
}

/*
    in case the request header contains transfer-encoding: chunked
    the chuned line that contains the chunk size are removed from the
    body of the response
*/
void requestParse::converChunkedRequest(void)
{
    if (this->data["transfer-encoding"] != "Chunked")
        return;

    std::istringstream f(this->body.content);
    std::string new_body;
    std::string line;

    while (std::getline(f, line))
    {
        if (line == "0")
            break;

        if (isChunkLine(line))
        {
            size_t chunk_size;
            std::istringstream iss(line);
            iss >> std::hex >> chunk_size;
            std::string chunk_data(chunk_size, '\0');
            f.read(&chunk_data[0], chunk_size);
            new_body += chunk_data;
            f.ignore(2);
        }
        else
        {
            new_body += line;
            f.ignore(2);
        }
    }

    this->body.content = new_body;
    this->data["content-length"] = std::to_string(new_body.size());
}

/*
    returns a requestParse object which contains the
    request of a given client parsed after reading it
*/
requestParse Server::getRequest(const Client &_client)
{
    fcntl(_client.socket, F_SETFL, O_NONBLOCK);
    int bytesRead;
    size_t bytesLeft;
    char buff[MAX_REQUEST_SIZE];
    std::string header;
    size_t totalSize = 0;
    while ((bytesRead = recv(_client.socket, buff, MAX_REQUEST_SIZE, 0)) > 0)
    {
        header.append(std::string(buff, bytesRead));
        totalSize += bytesRead;
    }
    requestParse request;
    request.setUp(header);
    bytesLeft = atoi(request.data["content-length"].c_str());
    int BUFFER_SIZE = 512;
    float factor = 0;
    if (bytesLeft == 0 && request.data["transfer-encoding"] != "Chunked")
    {
        return request;
    }
    else if ((bytesLeft * 0.000001) <= 100)
        factor += 0.25;
    else if ((bytesLeft * 0.000001) <= 300)
        factor += 0.50;
    else
        factor += 1;
    BUFFER_SIZE *= factor;
    char c[BUFFER_SIZE];
    int i = 0;
recvAgain:
    while (true)
    {
        if ((bytesRead = recv(_client.socket, c, BUFFER_SIZE, 0)) < 0)
            break;
        if (bytesRead == 0)
            return request;
        request.body.content += std::string(c, bytesRead);
        i += bytesRead;
    }
    if (request.data["transfer-encoding"] == "Chunked")
    {
        if (request.body.content.find("\r\n0\r\n") == std::string::npos)
            goto recvAgain;
    }
    else if (request.body.content.size() < bytesLeft)
        goto recvAgain;
    request.converChunkedRequest();
    return request;
}

/*
    sends the response to all the clients that have requests
    queued and able to write to their sockets the connection
    is closed after the client have been served
*/
void Server::serveContent()
{
    signal(SIGPIPE, SIG_IGN);
    std::vector<Client>::iterator it = _clients.begin();
    while (it != _clients.end())
    {
        if (FD_ISSET(it->socket, &_readyToReadFrom))
        {
            fcntl(it->socket, F_SETFL, O_NONBLOCK);
            it->request = getRequest(*it);
            it->response.setUp(_configFile, it->request);
            int ret;
            int i = 0;
            it->remaining = it->response._response.size();
            it->received = 0;
            while (it->remaining > 0)
            {
                int chunk = std::min(it->remaining, MAX_CHUNK_SIZE);
                char buff[chunk];
                for (int j = 0; j < chunk; ++j)
                    buff[j] = it->response._response[i++];
                if (FD_ISSET(it->socket, &_readyToWriteTo))
                {
                    ret = send(it->socket, buff, chunk, 0);
                    if (ret < 0)
                        break;
                    it->remaining -= ret;
                    it->received += ret;
                }
            }
            close(it->socket);
            it = _clients.erase(it);
        }
        else
            it++;
    }
}

/*
    a constructor which calls all the helper methods
    and starts the server main loop
*/
Server::Server(std::string file) : _configFile(file)
{
    srand(time(NULL));
    std::vector<std::string> _ports;
    std::cout << this->_configFile.servers[4].autoIndex << std::endl;
    for (size_t i = 0; i < _configFile.servers.size(); ++i)
    {
        if (_configFile.servers[i].data["listen"].size() >= 1)
        {
            initServerSocket(_configFile.servers[i].data["listen"].front().c_str());
            std::cout << "Server: " << i << " is listening on "
                      << "http://localhost:"
                      << _configFile.servers[i].data["listen"].front().c_str() << std::endl;
        }
        // int port = (rand() % (65535 - 1024 + 1)) + 1024;
    }

    while (1)
    {
        for (size_t i = 0; i < _serverSockets.size(); i++)
        {
            acceptConnection(i);
            serveContent();
            waitForClients();
        }
    }
    for (size_t i = 0; i < _serverSockets.size(); i++)
        close(_serverSockets[i]);
}

int main(int ac, char **av)
{
    if (ac == 2)
        Server server(av[1]);
}
