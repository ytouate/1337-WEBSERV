#include "Server.hpp"

Server::Server(std::string s)
{
    configFile.open(s, std::fstream::in);
    std::string buff;
    while (std::getline(configFile, buff))
    {
        buff = trim(buff);
        if (buff != "")
            fileBuff.push_back(buff);
    }
    if (fileBuff.size() <= 2)
        error("Invalid Config File");
    configFile.close();
    this->parseBlock();
    data.insert(_index);
    data.insert(_root);
    data.insert(_port);
    serverIsOpened = false;
    locationIsOpened = false;
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
    {
        key += fileBuff[j++];
    }
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

void Server::parseBlock()
{
    size_t i = 0;
    int j = 0;
    i = 0;
    std::string key;
    std::string val;
    int locationsCount = 0;
    std::vector<std::string> values;
    isInsideServer = true;
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