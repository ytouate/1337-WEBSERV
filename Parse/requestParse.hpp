#if !defined(requestParse_HPP)
#define requestParse_HPP

#include "../inc.hpp"


struct Body
{
    std::string content;
    std::string contentType;
    std::string contentName;
    void setUp();

    private:
    void getFileName();
    void getContentType();
    void trimUnwantedLines();
};

class requestParse
{
private:
    void    parseRequestLine(std::string &s, const std::string &delimiter);
    void    getHost(std::string &s);
public:
void    converChunkedRequest();
    std::string                             requestString;
    void setRequest(std::string);
    std::map<std::string, std::string>      data;
    Body                                    body;
    requestParse();
    void setUp(std::string);
    ~requestParse();
};

#endif // requestParse_HPP