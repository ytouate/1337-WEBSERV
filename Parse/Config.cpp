#include "Config.hpp"

Config::Config(std::string s)
{
    readFileIntoBuff(s);
    size_t end = 0;
    while (end < _fileBuff.size())
    {
        serverParse server = serverParse(_fileBuff, end);
        end = server.parseBlock();
        this->servers.push_back(server);
    }
}

std::string Config::trim(const std::string &s)
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

void Config::readFileIntoBuff(const std::string &s)
{
    _configFile.open(s.c_str(), std::fstream::in);
    std::string tmp;
    while (std::getline(_configFile, tmp))
    {
        tmp = trim(tmp);
        if (!tmp.empty())
            _fileBuff.push_back(tmp);
    }
    _configFile.close();
}
Config::~Config() {}

bool Config::isWhiteSpace(char c)
{
    return (c == ' ' or c == '\t');
}

void Config::error(const std::string &s) const
{
    std::cerr << s << std::endl;
    exit(EXIT_FAILURE);
}