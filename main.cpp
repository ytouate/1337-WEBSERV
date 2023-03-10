#include <iostream>

#include <vector>
#include <map>
#include <fstream>

#include <list>
#include <vector>
#include <map>
#include "Lexer.hpp"
#include "Server.hpp"
typedef std::map<std::string, std::vector<std::string> > directive;
int error(const std::string &_err)
{
    std::cerr << _err << std::endl;
    return -1;
}

bool isWhiteSpace(char c)
{
    return (c == ' ' or c == '\n' or c == '\t');
}

bool isServerBlock(const std::string &s)
{
    size_t i = 0;
    std::string content;
    while (s[i] && isWhiteSpace(s[i]))
        i++;
    while (s[i])
    {
        if (!isWhiteSpace(s[i]))
            content += s[i];
        i++;
    }
    return content == "server{";
}


directive getDirective(std::string s)
{
    int i = 0;
    int j = s.size() - 1;
    directive _directive;
    while (s[i] && isWhiteSpace(s[i]))
        i++;
    std::string key;
    std::vector<std::string> values;
    std::string val;
    while (j < i && s[i] && !isWhiteSpace(s[i]))
        key += s[i++];
    // i++;
    while (s[i])
    {
        while (isWhiteSpace(s[i]))
        {
            values.push_back(val);
            val = "";
            i++;
        }
        val += s[i];
        i++;
        if (!s[i])
            values.push_back(val);
    }
    _directive.insert(std::make_pair(key, values));
    return _directive;
}

bool isClosingBracket(const std::string &s)
{
    int i = 0;
    while (isWhiteSpace(s[i]))
        i++;
    return s[i] == '}';
}
int main(int ac, char **av)
{
    if (ac != 2)
        return error("Not Enough Arguments");
    std::fstream configFile;
    Token *token = NULL;
    configFile.open(av[1], std::fstream::in);
    std::string line;
    std::string Buff;
    std::vector<directive> vec;
    bool isFirstLine = true;
    while (std::getline(configFile, line))
    {
        if (isFirstLine)
        {
            if (isServerBlock(line))
            {
                isFirstLine = false;
                continue;
            }
            else
                return error("Invalid config file");
        }
        else
        {
            if (!isClosingBracket(line))
                vec.push_back(getDirective(line));
        }
    }
    for (int i = 0; i < vec.size(); i++)
    {
        directive::iterator it = vec[i].begin();
        directive::iterator ite = vec[i].end();
        std::cout << "key: " << it->first << " - values: ";
        for (int i = 0; i < it->second.size(); ++i)
            std::cout << it->second.at(i) << " ";
        std::cout << std::endl;
    }
}