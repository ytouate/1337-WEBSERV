#if !defined(SERVER_HPP)
#define SERVER_HPP

#include "Location.hpp"
#include <map>

struct Server
{
    Location location;
    std::map<std::string, std::vector<std::string> > _port;
    std::map<std::string, std::vector<std::string> > _root;
    std::map<std::string, std::vector<std::string> > _index;
};

#endif // SERVER_HPP
