#if !defined(requestParse_HPP)
#define requestParse_HPP

#include <iostream>
#include <map>

class requestParse
{
private:
    void    parseRequestLine(std::string &s, const std::string &delimiter);
    void    getHost(std::string &s);
public:
    std::map<std::string, std::string>      data;

    requestParse(std::string);
    ~requestParse();
};

#endif // requestParse_HPP