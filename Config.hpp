#if !defined(ACONFIG_HPP)
#define ACONFIG_HPP

#include <iostream>
#include <map>
#include <vector>
#include "Server.hpp"
#include <fstream>
class Server;
class Config
{ 
protected:
    std::pair<std::string, std::vector<std::string> > _root;
    std::pair<std::string, std::vector<std::string> > _index;
    std::pair<std::string, std::vector<std::string> > _allowed_methods;
    std::pair<std::string, std::vector<std::string> > _port;
    std::fstream configFile;
    std::vector<std::string> fileBuff;
    void readFileIntoBuff(const std::string &s);
    bool isWhiteSpace(char c);
    void error(const std::string &a) const;
    std::string trim(const std::string &s); 

public:
    std::vector<Server> servers;
    std::map<std::string, std::vector<std::string> > data;
    Config(std::string s);
    virtual ~Config();
};

#endif // ACONFIG_HPP
