#if !defined(REQUEST_HPP)
#define REQUEST_HPP

#include <iostream>
#include <map>

class Request
{
private:
public:
    std::map<std::string, std::string> data;
    Request(std::string);
    void parseRequestLine(std::string &s, const std::string &delimiter);
    ~Request();
    void getHost(std::string &s);
};

#endif // REQUEST_HPP