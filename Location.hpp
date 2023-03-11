#if !defined(LOCATION_HPP)
#define LOCATION_HPP

#include <iostream>
#include <set>
#include <map>
#include <vector>

struct Location
{
    std::string path;
    std::vector<std::string> fileBuff;
    std::pair<std::string, std::vector<std::string> > _root;
    std::pair<std::string, std::vector<std::string> > _index;
    Location();
    void parseLocation();
    void trimFile();
    void collectPath(const std::string &s);
    bool isWhiteSpace(char c);
    void fillDirective(const std::string &s, const std::string &key);
    void error(const std::string &a);
};

// void Location::parseLocation(std::string *fileBuff, int fileSize)
// {

// }

#endif // LOCATION_HPP
