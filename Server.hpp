#if !defined(SERVER_HPP)
#define SERVER_HPP

#include "Location.hpp"
#include <map>
#include <fstream>
#include <vector>
#include <iostream>


class Server
{
    private:
        std::fstream configFile;
        std::string lastKey;
        bool serverIsOpened;
        bool locationIsOpened;
        std::vector<std::string> fileBuff;
        std::vector<std::string> locationBuff;
        std::string trim(const std::string &s);
        bool isWhiteSpace(char c);
        void parseDirective();
        void fillDirective(const std::string &, const std::vector<std::string> &);
        void error(const std::string &) const;
        typedef std::pair<std::string, std::vector<std::string> > directive;

    public:
    Location location;
    directive _port;
    directive _root;
    directive _index;
    Server(std::string s);
    ~Server();
};

#endif // SERVER_HPP
