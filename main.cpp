#include <iostream>

#include "Server.hpp"

typedef std::map<std::string, std::vector<std::string> > directive;
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>

void printDrective(directive &a)
{
    std::cout << "root:\t";
    for (size_t i = 0; i < a["root"].size(); ++i)
        std::cout << " " << a["root"][i];
    std::cout << "\n";
    std::cout << "index:\t";
    for (size_t i = 0; i < a["index"].size(); ++i)
        std::cout << " " << a["index"][i];
    std::cout << "\n";
    std::cout << "listen:\t";
    for (size_t i = 0; i < a["listen"].size(); ++i)
        std::cout << " " << a["listen"][i];
    std::cout << "\n";
}
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
// int main(int ac, char **av)
// {
//     if (ac != 2)
//         return 1;
//     Server server(av[1]);
//     std::cout << "=== Server Block data === " << std::endl;
//     printDrective(server.data);
//     std::cout << "=== Location[0] data === " << std::endl;
//     std::cout << server.locations[0].path << std::endl;
//     printDrective(server.locations[0].data);
//     std::cout << "=== Location[1] data === " << std::endl;
//     std::cout << server.locations[1].path << std::endl;
//     printDrective(server.locations[1].data);
//     std::cout << "=== Location[2] data === " << std::endl;
//     std::cout << server.locations[2].path << std::endl;
//     printDrective(server.locations[2].data);

//     return 0;
// }
#include <unistd.h>


void error(const char *s)
{
    perror(s);
    exit(1);
}
// int main()
// {
//     int errorCode = 0;
//     struct addrinfo hints, *res;
//     std::memset(&hints, 0, sizeof (hints));
//     // hints.ai_family = AF_UNSPEC;
//     hints.ai_socktype = SOCK_STREAM;
//     errorCode = getaddrinfo("localhost", "8080", &hints, &res);
//     if (errorCode != 0)
//     {
//         std::cout << "getaddrinfo() failed with: " <<  gai_strerror(errorCode) << std::endl;
//         exit(1);
//     }

//     int sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
//     if (sock == -1)
//         error("socket()");

//     // errorCode = bind(sock, res->ai_addr, res->ai_addrlen);
//     // if (errorCode != 0)
//     //     error("bind()");

//     /*
//         to connect to a remote site
//     */
//     // errorCode = connect(sock, res->ai_addr, res->ai_addrlen);
//     // if (errorCode != 0)
//     //     error("connect()");
//     errorCode = listen(sock, 10);
//     if (errorCode != 0)
//         error("listen()");

//     // std::string msg = "hello wrold";
//     // int bytes = send(sock, msg.c_str(), msg.size(), 0);
//     // char buf[100];
//     // bytes = recv(sock, buf, bytes, 0);
//     // std::cout << buf << std::endl;
//     // std::cout << bytes << std::endl;
//     // shutdown(sock, 2);
//     // if (close(sock) == -1)
//     //     error("close()");
//     // freeaddrinfo(res);
//     // return 0;
//     /*
//         to accept connections
//     */
//     struct sockaddr theirAddr;
//     socklen_t addSize = sizeof theirAddr;
//     getpeername(sock, &theirAddr, &addSize);
//     char buf[100];
//     gethostname(buf, 100);
//     std::cout << buf << std::endl;
//     // int newSock = accept(sock, (struct sockaddr *)&theirAddr, &addSize);
//     // std::cout << newSock << std::endl;
//     // if (newSock == -1)
//     //     error("accept()");
//     return 0;
// }
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

int main()
{
    struct addrinfo hints, *data;

    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_family = AF_UNSPEC;
    hints.ai_flags = AI_PASSIVE;

    getaddrinfo(NULL, "8080", &hints, &data);

    int sockfd;
    if ((sockfd = socket(data->ai_family, data->ai_socktype, data->ai_protocol)) == -1 )
    {
        perror("socket()");
        return 1;
    }

    if (bind(sockfd, data->ai_addr, data->ai_addrlen) == -1)
        perror("bind()");
    
    if (listen(sockfd, 10) == -1)
        perror("listen()");
    
    std::cout << "Server is waiting for connection" << std::endl;

    while (1)
    {
        struct sockaddr theirAddr;
        socklen_t size = sizeof (theirAddr);
        int new_fd;
        if ((new_fd = accept(sockfd, &theirAddr, &size)) == -1)
        {
            perror("accept()");
            return 1;
        }
        if (!fork())
        {
            close(sockfd);
            if (send(new_fd, "hello world", 11, 0) == -1)
            {
                perror("send");
                return 1;
            }
            close(new_fd);
            exit(0);
        }
        close(new_fd);
    }
}