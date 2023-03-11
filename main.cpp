#include <iostream>

#include "Server.hpp"

typedef std::pair<std::string, std::vector<std::string> > directive;

void printDirective(const directive &d)
{
    std::cout << "key: " << d.first<< " --- values: ";
    for (size_t i = 0; i < d.second.size(); i++)
        std::cout << " " << d.second[i];
    std::cout << "\n";
}
#include <unistd.h>

int main(int ac, char **av)
{

    if (ac != 2)
        return 1;
    Server server(av[1]);
    std::cout << server.locations[0].path << std::endl;
    printDirective(server.locations[0]._index);
    // printDirective(server.locations[0]._root);
    std::cout << server.locations[1].path << std::endl;
    printDirective(server.locations[1]._index);
    // printDirective(server.locations[1]._root);
    // printDirective(server._index);
    // printDirective(server._port);
    // printDirective(server._root);
    
}