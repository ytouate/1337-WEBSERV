#include <iostream>
#include "Server.hpp"
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

int main(int ac, char **av)
{
    int sockfd;
    struct addrinfo hints, *data;
    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_family = AF_UNSPEC;

    getaddrinfo(av[1], "8080", &hints, &data);
    if ((sockfd = socket(data->ai_family, data->ai_socktype, data->ai_protocol)) == -1)
    {
        perror("socket()");
        return 1;
    }
    if (connect(sockfd, data->ai_addr, data->ai_addrlen) == -1)
    {
        perror("connect()");
        return 1;
    }

    std::cout << "client: connected\n";
    char buf[101];
    int numBytes = recv(sockfd, buf, 100, 0);
    if (numBytes == -1)
    {
        perror("recv()");
        return 1;
    }
    std::cout << buf << std::endl;
    close(sockfd);

}