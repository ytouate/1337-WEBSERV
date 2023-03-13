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

int main(int ac, char **av)
{
    // struct ifaddrs *addresses;
    // if (getifaddrs(&addresses) == -1)
    //     std::cout << "failed\n";
    // while (addresses)
    // {
    //     int family = addresses->ifa_addr->sa_family;
    //     if (family == AF_INET)
    //     {
    //         std::cout << addresses->ifa_name << std::endl;
    //         char ap[100];
    //         const int family_size = sizeof(struct sockaddr_in);
    //         getnameinfo(addresses->ifa_addr, family_size, ap, sizeof(ap),
    //         0, 0, NI_NUMERICHOST);
    //         std::cout << ap << std::endl;
    //     }
    //     addresses = addresses->ifa_next;
    // }
    // free(addresses);
    // return 0;
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