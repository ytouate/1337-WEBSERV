#include "inc.hpp"
#include <map>
#include <iostream>
#include <vector>
#include "Parse/Config.hpp"
#include "Response/Response.hpp"
#include <deque>
#include <fcntl.h>

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


