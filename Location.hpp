#if !defined(LOCATION_HPP)
#define LOCATION_HPP

#include <iostream>
#include <set>
#include <map>
#include <vector>
#include "AConfig.hpp"

class Location : public AConfig
{
private:
    int start;
    std::vector<std::string> fileBuff;
    void collectPath(const std::string &s);
    void parseBlock();
    void fillDirective(const std::string &s, const std::string &key);
    friend class Server;

public:
    std::string path;
    Location(const std::vector<std::string> &, int);
};

#endif // LOCATION_HPP
