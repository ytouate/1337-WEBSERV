#if !defined(ACONFIG_HPP)
#define ACONFIG_HPP

#include "inc.hpp"
#include "Server.hpp"

class Server;
class Config
{
protected:
    std::fstream                    _configFile;
    std::vector<std::string>        _fileBuff;

    void            readFileIntoBuff(const std::string &s);
    void            error(const std::string &a) const;
    bool            isWhiteSpace(char c);
    std::string     trim(const std::string &s);

public:
    std::vector<Server>             servers;

    Config(std::string s);
    ~Config();
};

#endif // ACONFIG_HPP
