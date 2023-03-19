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

int main()
{
    struct addrinfo hints, *data;

    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_family = AF_UNSPEC;
    hints.ai_flags = AI_PASSIVE;

    getaddrinfo(NULL, "8080", &hints, &data);

    int sockfd;
    if ((sockfd = socket(data->ai_family, data->ai_socktype, data->ai_protocol)) == -1)
    {
        perror("socket()");
        return 1;
    }

    if (bind(sockfd, data->ai_addr, data->ai_addrlen) == -1)
    {
        perror("bind()");
        exit(1);
    }

    if (listen(sockfd, 10) == -1)
        perror("listen()");
    
    struct sockaddr_in *ip = (struct sockaddr_in *)&data->ai_addr;
    struct in_addr ipAddr = ip->sin_addr;

    std::cout << "Server is waiting for connection at: " << std::endl;

    while (1)
    {
        struct sockaddr theirAddr;
        socklen_t size = sizeof(theirAddr);
        int new_fd;
        if ((new_fd = accept(sockfd, &theirAddr, &size)) == -1)
        {
            perror("accept()");
            return 1;
        }

        std::cout << "Server got the connection\n";
        if (!fork())
        {
            close(sockfd);
            if (send(new_fd, "hello world\n", 12, 0) == -1)
            {
                perror("send");
                return 1;
            }
            close(new_fd);
            exit(0);
        }
        shutdown(new_fd, 0);
        close(new_fd);
    }
}