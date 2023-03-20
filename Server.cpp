#include "Server.hpp"

Server::Server(const std::vector<std::string> &_fileBuff, int _start)
{
    lastKey = "";
    serverIsOpened = locationIsOpened = false;
    isInsideServer = false;
    fileBuff = _fileBuff;
    start = _start;
}


bool isCurlyBracket(const std::string &s)
{
    return (s == "{" or s == "}");
}

void Server::handleErrors(const std::string &fileBuff)
{
    if (isCurlyBracket(fileBuff) or fileBuff == "server")
    {
        if (fileBuff == "{")
        {
            if (lastKey == "server")
                serverIsOpened = true;
            else if (lastKey == "location")
                locationIsOpened = true;
            else
                error("invalid config file");
        }
        else if (fileBuff == "}")
        {
            if (locationIsOpened)
                locationIsOpened = false;
            else if (serverIsOpened)
                serverIsOpened = false;
            else
                error("invalid config file");
        }
    }
}

std::string Server::getKey(const std::string &fileBuff, int &j)
{
    std::string key;
    while (fileBuff[j] && !isWhiteSpace(fileBuff[j]))
        key += fileBuff[j++];
    return key;
}

void Server::getValues(std::vector<std::string> &values,
                       const std::string &fileBuff, int &j)
{
    std::string val;
    while (fileBuff[j])
    {
        while (fileBuff[j] && isWhiteSpace(fileBuff[j]))
            j++;
        val += fileBuff[j];
        j++;
        if (!fileBuff[j] or isWhiteSpace(fileBuff[j]))
        {
            values.push_back(val);
            val = "";
        }
    }
}

int Server::parseBlock()
{
    size_t i = start;

    int j = 0;
    std::string key;
    std::string val;
    int locationsCount = 0;
    std::vector<std::string> values;
    isInsideServer = false;
    // std::cout << fileBuff.size() << std::endl;
    while (i < fileBuff.size())
    {
        isInsideServer = !locationIsOpened;
        j = 0;
        values.clear();
        lastKey = key;
        val = key = "";
        handleErrors(fileBuff[i]);
        key = getKey(fileBuff[i], j);
        if ((key == "server" && fileBuff[i + 1] != "{") ||
            (key == "location" && fileBuff[i + 1] != "{"))
            error("block not opened");
        if (key == "location")
        {
            if (locationIsOpened)
                error("unclosed block");
            locations.push_back(Location(this->fileBuff, i));
            locations[locationsCount].parseBlock();
            locationsCount++;
        }
        getValues(values, fileBuff[i], j);
        fillDirective(key, values);
        i++;
        if (key == "}" and lastKey == "}")
            break;
    }
    for (size_t i = 0; i < locations.size(); i++)
    {
        if (locations[i].data["root"].size() == 0)
            locations[i].data["root"] = this->serverRoot.second;
        else if (locations[i].data["index"].size() == 0)
            locations[i].data["index"] = this->serverIndex.second;
    }
    if (serverIsOpened or locationIsOpened)
        error("block not closed");
    data.insert(_index);
    data.insert(_root);
    data.insert(_port);
    data.insert(_server_name);
    // std::cout << "dkhel " << i << std::endl;
    return i;
}

bool Server::isWhiteSpace(char c)
{
    return (c == ' ' or c == '\t');
}

void Server::error(const std::string &s) const
{
    std::cerr << s << std::endl;
    exit(EXIT_FAILURE);
}
void Server::fillDirective(const std::string &key,
                           const std::vector<std::string> &values)
{
    if (key == "listen")
        _port = std::make_pair(key, values);
    else if (key == "root")
    {
        if (isInsideServer)
            this->serverRoot = std::make_pair(key, values);
        _root = std::make_pair(key, values);
    }
    else if (key == "index")
    {
        if (isInsideServer)
            this->serverIndex = std::make_pair(key, values);
        _index = std::make_pair(key, values);
    }
    else if (key == "server_name")
    {
        _server_name = std::make_pair(key, values);
    }
    else
    {
        if (key != "location" && key != "server" && !isCurlyBracket(key))
            error("invalid directive"); 
    }
}

std::string Server::trim(const std::string &s)
{
    std::string trimmed;
    int start = 0;
    int end = s.size() - 1;
    if (end < 0)
        return "";
    while (s[start] && isWhiteSpace(s[start]))
        start++;
    while (end > 0 && isWhiteSpace(s[end]))
        end--;
    while (start <= end)
        trimmed += s[start++];
    return trimmed;
}

Server::~Server() {}
