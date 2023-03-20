#if !defined(REQUEST_HPP)
#define REQUEST_HPP

#include <iostream>
#include <map>

class Request
{
private:
    void parseRequestLine(std::string &s, const std::string &delimiter);
    void getHost(std::string &s);
public:
    std::map<std::string, std::string> data;
    Request(std::string);
    ~Request();
};

#endif // REQUEST_HPP