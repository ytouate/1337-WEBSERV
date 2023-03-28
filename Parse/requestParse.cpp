#include "requestParse.hpp"

void requestParse::parseRequestLine(std::string &s, const std::string &delimiter)
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

void requestParse::getHost(std::string &s)
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
        s = s.erase(0, pos + 1);
        this->data.insert(std::make_pair("host", s));
    }
}

requestParse::requestParse(std::string _requestParse)
{
    size_t pos = 0;
    std::string buff;
    std::string token;
    bool isrequestParseLine = true;
    while ((pos = _requestParse.find("\n")) != std::string::npos)
    {
        token = _requestParse.substr(0, pos);
        if (isrequestParseLine)
        {
            isrequestParseLine = false;
            parseRequestLine(token, " ");
            continue;
        }
        this->getHost(token);

        _requestParse.erase(0, pos + 1);
    }
}

requestParse::~requestParse()
{
    this->data.clear();
}