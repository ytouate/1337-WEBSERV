#if !defined(SERVER_HPP)
#define SERVER_HPP

#include "Location.hpp"
#include <map>
#include <fstream>
#include <vector>
#include <iostream>
#include "AConfig.hpp"

class Server : public AConfig
{
private:
    std::fstream configFile;
    std::string lastKey;
    bool serverIsOpened;
    bool locationIsOpened;
    bool isInsideServer;

    std::vector<std::string> fileBuff;
    std::vector<std::string> locationBuff;
    std::string trim(const std::string &s);
    void parseBlock();
    std::pair<std::string, std::vector<std::string> > serverRoot;
    void fillDirective(const std::string &, const std::vector<std::string> &);
    void handleErrors(const std::string &);
    std::string getKey(const std::string &fileBuff, int &j);
    void getValues(std::vector<std::string > &values, const std::string &fileBuff, int &j);
public:
    std::vector<Location> locations;
    std::map<std::string, std::vector<std::string> > data;
    Server(std::string s);
    ~Server();
};

#endif // SERVER_HPP
