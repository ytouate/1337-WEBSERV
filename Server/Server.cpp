#include "../inc.hpp"
#include "Server.hpp"
#include "../Parse/requestParse.hpp"
#include "../Response/Response.hpp"
#include "../Parse/serverParse.hpp"

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

/*
    adds new client to the _readyToWriteTo and _readyToReadFrom fd_sets
    and handles communication with already connected clients using select
*/
void Server::waitForClients()
{
    FD_ZERO(&_readyToReadFrom);
    FD_ZERO(&_readyToWriteTo);
    FD_SET(_serverSocket, &_readyToReadFrom);
    int maxSocket = _serverSocket;
    std::vector<Client>::iterator it = _clients.begin();
    while (it != _clients.end())
    {
        FD_SET(it->socket, &_readyToReadFrom);
        FD_SET(it->socket, &_readyToWriteTo);
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
    }
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

    requestParse request(header);
    bytesLeft = atoi(request.data["content-length"].c_str());
    int BUFFER_SIZE = 512;
    float factor = 0;
    if (bytesLeft == 0)
        return request;
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
    if (request.body.content.size() < bytesLeft)
        goto recvAgain;
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
            requestParse request = getRequest(*it);
            Response response(_configFile, request);
            it->remaining = response._response.size();
            it->received = 0;
            int ret;
            int i = 0;
            while (it->remaining > 0)
            {
                int chunk = std::min(it->remaining, MAX_CHUNK_SIZE);
                char buff[chunk];
                for (int j = 0; j < chunk; ++j)
                {
                    buff[j] = response._response[i];
                    ++i;
                }
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
Server::Server(std::string file, char **_env) : _configFile(file)
{
    env = _env;
    srand(time(NULL));
    int port = (rand() % (65535 - 1024 + 1)) + 1024;

    initServerSocket(std::to_string(port).c_str());
    std::cout << "http://localhost:" << port << std::endl;
    while (1)
    {
        waitForClients();
        acceptConnection();
        serveContent();
    }
    close(_serverSocket);
}

int main(int ac, char **av, char **env)
{
    if (ac == 2)
        Server server(av[1], env);
}
