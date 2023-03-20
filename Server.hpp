#if !defined(SERVER_HPP)
#define SERVER_HPP

#include <map>
#include <fstream>
#include <vector>
#include <iostream>
#include "Location.hpp"


class Location;
class Server
{
private:
    std::string lastKey;
    bool serverIsOpened;
    bool locationIsOpened;
    int start;
    bool isInsideServer;
    std::pair<std::string, std::vector<std::string> > _root;
    std::pair<std::string, std::vector<std::string> > _index;
    std::pair<std::string, std::vector<std::string> > _allowed_methods;
    std::pair<std::string, std::vector<std::string> > _port;
    std::pair<std::string, std::vector<std::string> > _server_name;
    void error(const std::string &a) const;
    std::pair<std::string, std::vector<std::string> > serverIndex;
    std::vector<std::string> fileBuff;
    std::vector<std::string> locationBuff;
    bool isWhiteSpace(char c);
    std::string trim(const std::string &s);
    int parseBlock();
    std::pair<std::string, std::vector<std::string> > serverRoot;
    void fillDirective(const std::string &, const std::vector<std::string> &);
    void handleErrors(const std::string &);
    std::string getKey(const std::string &fileBuff, int &j);
    friend class Config;
    void setFileBuff(const std::vector<std::string> &);
    void getValues(std::vector<std::string > &values, const std::string &fileBuff, int &j);
public:
    std::vector<Location> locations;
    std::map<std::string, std::vector<std::string> > data;
    Server(const std::vector<std::string> &_fileBuff, int);
    ~Server();
};

#endif // SERVER_HPP
