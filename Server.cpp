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
    location.fileBuff = fileBuff;
    location.parseLocation();
    parseDirective();
    serverIsOpened = false;
    locationIsOpened = false;
}

void Server::error(const std::string &s) const
{
    std::cerr << s << std::endl;
    exit(EXIT_FAILURE);
}
bool isCurlyBracket(const std::string &s)
{
    return (s == "{" or s == "}");
}
void Server::parseDirective()
{
    size_t i = 0;
    int j = 0;
    i = 0;
    std::string key;
    std::string val;
    std::vector<std::string> values;
    while (i < fileBuff.size())
    {
        j = 0;
        values.clear();
        val.clear();
        lastKey = key;
        key.clear();
        if (isCurlyBracket(fileBuff[i]))
        {
            if (fileBuff[i] == "{")
            {
                if (lastKey == "server")
                    serverIsOpened = true;
                else if (lastKey == "location")
                    locationIsOpened = true;
                else
                    error("Invalid config file check braces");
            }
            else
            {
                if (locationIsOpened)
                    locationIsOpened = false;
                else if (serverIsOpened)
                    serverIsOpened = false;
                else
                    error("Invalid config file check braces");
            }
        }
        while (fileBuff[i][j] && !isWhiteSpace(fileBuff[i][j]))
            key += fileBuff[i][j++];
        if ((key == "server" && fileBuff[i + 1] != "{") ||
            (key == "location" && fileBuff[i + 1] != "{"))
            error("Server/Location Block not opened");
        while (fileBuff[i][j])
        {
            while (isWhiteSpace(fileBuff[i][j]))
                j++;
            val += fileBuff[i][j];
            j++;
            if (!fileBuff[i][j] or isWhiteSpace(fileBuff[i][j]))
            {
                values.push_back(val);
                val = "";
            }
        }
        i++;
        fillDirective(key, values);
    }
    if (serverIsOpened or locationIsOpened)
        error("Block not closed\n");
}
void Server::fillDirective(const std::string &key,
    const std::vector<std::string> &values)
{
    if (key == "listen")
        _port = std::make_pair(key, values);
    else if (key == "root")
        _root = std::make_pair(key, values);
    else if (key == "index")
        _index = std::make_pair(key, values);
}

bool Server::isWhiteSpace(char c)
{
    return (c == ' ' or c == '\n' or c == '\t');
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

Server::~Server()
{
}