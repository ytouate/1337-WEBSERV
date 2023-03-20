#include "Config.hpp"

Config::Config(std::string s)
{
    readFileIntoBuff(s);
    size_t end = 0;
    while (end < fileBuff.size())
    {
        Server server = Server(fileBuff, end);
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
    configFile.open(s, std::fstream::in);
    std::string tmp;
    while (std::getline(configFile, tmp))
    {
        tmp = trim(tmp);
        if (!tmp.empty())
            fileBuff.push_back(tmp);
    }
    configFile.close();
}
Config::~Config()
{
    data.clear();
}

bool Config::isWhiteSpace(char c)
{
    return (c == ' ' or c == '\t');
}

void Config::error(const std::string &s) const
{
    std::cerr << s << std::endl;
    exit(EXIT_FAILURE);
}