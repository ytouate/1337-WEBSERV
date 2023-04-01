#if !defined(requestParse_HPP)
#define requestParse_HPP

#include "../inc.hpp"


struct Body
{
    std::string content;
    std::string contentType;
    std::string contentName;

    void setUp();
};

class requestParse
{
private:
    void    parseRequestLine(std::string &s, const std::string &delimiter);
    void    getHost(std::string &s);

public:
    std::map<std::string, std::string>      data;
    Body                                    body;
    requestParse(std::string);
    ~requestParse();
};

#endif // requestParse_HPP