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
    if (token == "Content-Length")
    {
        pos = s.find_last_of(" ");
        s = s.erase(0, pos + 1);
        token = "";
        for (size_t i = 0; i < s.size(); ++i)
        {
            if (isdigit(s[i]))
                token += s[i];
            else
                this->data.insert(std::make_pair("content-length", token));
        }
    }
    else if (token == "Content-Type")
    {
        token = "";
        pos = s.find(" ");
        s = s.erase(0, pos + 1);
        pos = s.find_last_of(";");
        if (pos != std::string::npos)
            s = s.erase(pos, s.size() + 1);
        this->data.insert(std::make_pair("content-type", s));
    }
}

void trimFirstLastThreeLines(std::string& str)
{
    // Find the first newline character and skip the first three lines
    size_t pos = 0;
    for (int i = 0; i < 3; ++i) {
        pos = str.find("\n", pos) + 1;
        if (pos == std::string::npos) {
            return; // Less than three lines
        }
    }

    // Find the last newline character and skip the last three lines
    size_t last = str.find_last_of("\n");
    if (last != std::string::npos) {
        for (int i = 0; i < 2; ++i) {
            last = str.find_last_of("\n", last - 1);
            if (last == std::string::npos) {
                break;
            }
        }
    }

    // Remove the first and last three lines
    if (last != std::string::npos && pos <= last) {
        str = str.substr(pos, last - pos);
    }
}


void Body::setUp()
{
    size_t pos;
    pos = this->content.find("filename=\"");
    if (pos != std::string::npos)
    {
        pos += 10;
        for (size_t i = pos; this->content[i]; i++)
        {
            if (this->content[i] == '"')
                break;
            this->contentName += this->content[i];
        }
    }
    pos = this->content.find("Content-Type: ");
    if (pos != std::string::npos)
    {
        pos += 14;
        for (size_t i = pos; this->content[i]; ++i)
        {
            if (this->content[i] == '\n')
                break;
            this->contentType += this->content[i];
        }
    }
    trimFirstLastThreeLines(this->content);
}
requestParse::requestParse(std::string _requestParse)
{
    size_t pos = 0;
    std::string token;
    std::string buff(_requestParse);
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