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

typedef std::map<std::string, std::vector<std::string> > directive;

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

int main(int ac, char **av)
{
    if (ac != 2)
        return 1;
    Server server(av[1]);
    std::cout << "=== Server Block data === " << std::endl;
    printDrective(server.data);
    std::cout << "=== Location[0] data === " << std::endl;
    std::cout << server.locations[0].path << std::endl;
    printDrective(server.locations[0].data);
    std::cout << "=== Location[1] data === " << std::endl;
    std::cout << server.locations[1].path << std::endl;
    printDrective(server.locations[1].data);
    std::cout << "=== Location[2] data === " << std::endl;
    std::cout << server.locations[2].path << std::endl;
    printDrective(server.locations[2].data);
    return 0;
}

void error(const char *s)
{
    perror(s);
    exit(1);
}

