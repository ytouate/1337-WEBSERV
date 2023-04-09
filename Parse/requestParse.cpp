#include "requestParse.hpp"

void requestParse::setRequest(std::string s)
{
    this->requestString = s;
}
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

void Body::trimUnwantedLines()
{
    size_t pos = 0;
    for (int i = 0; i < 4; ++i)
    {
        pos = content.find("\n", pos) + 1;
        if (pos == std::string::npos)
            return;
    }
    content = std::string(content.c_str() + pos, content.size() - pos);
    size_t t = content.rfind("------WebKitFormBoundary");
    if (t != std::string::npos)
    {
        content.erase(t, content.size());
    }
}

void Body::setUp()
{
}

void Body::getFileName()
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
}

void Body::getContentType()
{
    size_t pos;

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
}

void requestParse::setUp(std::string _requestParse)
{
    requestString = _requestParse;
    std::stringstream ss(_requestParse);
    ss >> this->data["method"] >> this->data["path"] >> this->data["version"];
    std::string headerName, headerValue;
    headerName.clear();
    int count = 0;
    while (getline(ss, headerName, ':') && getline(ss, headerValue))
    {
        if (headerValue.size() > 3)
            headerValue = headerValue.substr(1, headerValue.size() - 2);
        size_t i;
        for (i = 0; i < headerName.size(); ++i)
            if (headerName[i] != '\n' && headerName[i] != '\r' && headerName[i] != ' ')
                break;
        headerName = headerName.substr(i);
        if (headerName == "Content-Length")
        {
            this->data["content-length"] = headerValue;
            ++count;
        }
        else if (headerName == "Host" || headerName == "\r")
        {
            this->data["host"] = headerValue;
            ++count;
        }
        else if (headerName == "Content-Type")
        {
            this->data["content-type"] = headerValue;
            ++count;
        }
        if (count == 3)
            break;
    }
    if (this->data["content-length"].empty() && this->data["method"] != "GET")
    {
        this->data["transfer-encoding"] = "Chunked";
    }
    size_t pos = _requestParse.find("\r\n\r\n");
    if (pos == std::string::npos)
        return;
    _requestParse = _requestParse.erase(0, pos + 4);
    this->body.content = _requestParse;
}
requestParse::requestParse()
{
}

requestParse::~requestParse()
{
    this->data.clear();
}