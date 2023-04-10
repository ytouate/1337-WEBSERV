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
        if (headerName == "Cookie")
        {
            size_t pos;
            while (true)
            {
                pos = headerValue.find(";");
                if (pos == std::string::npos)
                    break;
                std::string cookie = headerValue.substr(0, pos);
                size_t start = cookie.find_first_not_of(" ");
                size_t end = cookie.find_last_not_of(" ");
                cookie = cookie.substr(start, end);
                this->cookies.insert(cookie);
                headerValue.erase(0, pos + 1);
            }
            if (!headerValue.empty())
            {
                size_t start = headerValue.find_first_not_of(" ");
                size_t end = headerValue.find_last_not_of(" ");
                headerValue = headerValue.substr(start, end);
                this->cookies.insert(headerValue);
            }
            std::set<std::string>::iterator it = cookies.begin();
            while (it != cookies.end())
            {
                std::cout << *it << std::endl;
                ++it;
            }
        }
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