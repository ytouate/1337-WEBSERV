#if !defined(ACONFIG_HPP)
#define ACONFIG_HPP
#include <iostream>
#include <map>
#include <vector>

class AConfig
{
protected:
    std::pair<std::string, std::vector<std::string> > _root;
    std::pair<std::string, std::vector<std::string> > _index;
    std::pair<std::string, std::vector<std::string> > _allowed_methods;
    std::pair<std::string, std::vector<std::string> > _port;
    bool isWhiteSpace(char c);
    void error(const std::string &a) const;

public:
    std::map<std::string, std::vector<std::string> > data;
    AConfig();
    virtual ~AConfig();
    virtual void parseBlock() = 0;
};

#endif // ACONFIG_HPP
