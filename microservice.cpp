#include "inc.hpp"

int main(int ac, char **av)
{
    struct addrinfo hints, *data;

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    int errorCode = getaddrinfo(NULL, "8080", &hints, &data);
    if (errorCode != 0)
        std::cout << gai_strerror(errorCode) << std::endl;
    int sockfd = socket(data->ai_family, data->ai_socktype, data->ai_protocol);
    if (sockfd == -1)
    {
        perror("socket()");
        return 1;
    }
    if (bind(sockfd, data->ai_addr, data->ai_addrlen) != 0)
    {
        perror("bind()");
        return 1;
    }
    if (listen(sockfd, 10) != 0)
    {
        perror("listen()");
        return 1;
    }
    char addres_buffer[100];
    char service_buffer[100];

        getnameinfo(data->ai_addr, data->ai_addrlen, addres_buffer,
            100, service_buffer, 100, NI_NUMERICHOST);
    std::cout << "server is running on " << addres_buffer << ":" << service_buffer << std::endl;
    while (1)
    {
        fd_set writes;
        
        
        struct sockaddr new_addr;
        socklen_t new_addr_len = sizeof (new_addr);
        std::cout << "waiting for connection to be accepted\n";
        int new_sockfd = accept(sockfd, &new_addr, &new_addr_len);
        if (new_sockfd == -1)
        {
            perror("accept()");
            return 1;
        }
        FD_ZERO(&writes);
        FD_SET(new_sockfd, &writes);
        std::cout << "got connection\n";
        struct timeval time;
        time.tv_sec = 2;
        time.tv_usec = 0; 
        select(new_sockfd + 1, 0, &writes, 0, &time);
        if (FD_ISSET(new_sockfd, &writes))
        {
            char buff[100];
            int bytes;
            bytes = recv(new_sockfd, buff, sizeof buff, 0);
            if (bytes == -1)
            {
                perror("recv()");
                return 1;
            }
            std::string response;
            for (int i = 0; i < bytes; i++ )
                response.push_back(toupper(buff[i]));
            if (send(new_sockfd, response.c_str(), response.size(), 0) == -1)
            {
                perror("send()");
                return 1;
            }
        }
        close(new_sockfd);
    }
}