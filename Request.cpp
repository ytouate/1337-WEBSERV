#include "Request.hpp"
#include <fstream>
#include <sstream>

void Request::parseRequestLine(std::string &s, const std::string &delimiter)
{
    size_t pos = 0;
    std::string token;
    int count = 0;
    while ((pos = s.find(delimiter)) != std::string::npos)
    {
        token = s.substr(0, pos);
        if (count == 0)
            this->data.insert(std::make_pair("method", token));
        if (count == 1)
            this->data.insert(std::make_pair("path", token));
        count++;
        s.erase(0, pos + delimiter.size());
    }
    this->data.insert(std::make_pair("version", s));
}

void Request::getHost(std::string &s)
{
    size_t pos = 0;
    pos = s.find(":");
    std::string token;
    token = s.substr(0, pos);
    pos = token.find_last_of(" ");
    token.erase(0, pos + 1);
    if (token == "Host")
    {
        pos = s.find_last_of(" ");
        s.erase(0, pos + 1);
        this->data.insert(std::make_pair("host", s));
    }
    // s = token;
}
Request::Request(std::string _request)
{
    size_t pos = 0;
    std::string buff;
    std::string token;
    bool isRequestLine = true;
    while ((pos = _request.find("\n")) != std::string::npos)
    {
        token = _request.substr(0, pos);
        if (isRequestLine)
        {
            isRequestLine = false;
            parseRequestLine(token, " ");
            continue;
        }
        this->getHost(token);

        _request.erase(0, pos + 1);
    }
}

Request::~Request() {}