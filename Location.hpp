#if !defined(LOCATION_HPP)
#define LOCATION_HPP

#include <iostream>
#include <set>
#include <map>
#include <vector>
#include "Config.hpp"

class Location
{
private:
    std::pair<std::string, std::vector<std::string> > _root;
    std::pair<std::string, std::vector<std::string> > _index;
    std::pair<std::string, std::vector<std::string> > _allowed_methods;
    std::pair<std::string, std::vector<std::string> > _port;
    bool isWhiteSpace(char c);
    void error(const std::string &a) const;
    int start;
    std::vector<std::string> fileBuff;
    void collectPath(const std::string &s);
    void parseBlock();
    void fillDirective(const std::string &s, const std::string &key);
    friend class Server;

public:
    std::string path;
    std::map<std::string, std::vector<std::string> > data;
    Location(const std::vector<std::string> &, int);
};

#endif // LOCATION_HPP
