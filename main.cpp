#include "inc.hpp"
#include <map>
#include <iostream>
#include <vector>
#include "Parse/Config.hpp"

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
    std::cout << "server_name:\t";
    for (size_t i = 0; i < a["server_name"].size(); ++i)
        std::cout << " " << a["server_name"][i];
    std::cout << "\n";
}

void error(const char *s)
{
    perror(s);
    exit(1);
}
#include "Response/Response.hpp"
#include <deque>
int main(int ac, char **av)
{

    Response response;
    if (ac != 2)
        return 1;
    Config config(av[1]);

    for (size_t i = 0; i < config.servers.size(); ++i)
    {
        std::cout << "Server[" << i << "]" << std::endl;
        std::cout << "server_name: " << config.servers[i].data["server_name"][0] << std::endl;
        std::cout << "root: " << config.servers[i].data["root"][0] << std::endl;
        std::cout << "index: " << config.servers[i].data["index"][0] << std::endl;
        std::cout << "listen: " << config.servers[i].data["listen"][0] << std::endl;
        std::cout << "error_page: " << config.servers[i].errorPages[404] << std::endl;
        for (size_t j = 0; j < config.servers[i].locations.size(); ++j)
        {
            std::cout << "\nServer[" << i << "]"
                      << "location[" << j << "]\n";
            std::cout << "PATH: " << config.servers[i].locations[j].path << std::endl;
            std::cout << "root: " << config.servers[i].locations[j].data["root"][0] << std::endl;
            std::cout << "error_page: " << config.servers[i].locations[j].errorPages[500] << std::endl;
            std::cout << "error_page: " << config.servers[i].locations[j].errorPages[404] << std::endl;
            if (config.servers[i].locations[j].data["index"].size() > 0)
                std::cout << "index: " << config.servers[i].locations[j].data["index"][0] << std::endl;
        }
        std::cout << "\n";
    }
}
