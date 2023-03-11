#include "Location.hpp"

Location::Location() {}
bool Location::isWhiteSpace(char c)
{
    return (c == ' ' or c == '\n' or c == '\t');
}

void Location::collectPath(const std::string &s)
{
    size_t i = 0;
    while (s[i] && !isWhiteSpace(s[i]))
        i++;
    while (s[i] && isWhiteSpace(s[i]))
        i++;
    while (s[i])
        path += s[i++];
    
}

void Location::error(const std::string &s)
{
    std::cerr << s << std::endl;
    exit(EXIT_FAILURE);
}

void Location::fillDirective(const std::string &s, const std::string &key)
{
    size_t i = 0;
    std::string val;
    while (s[i] && !isWhiteSpace(s[i]))
        i++;
    while (s[i] && isWhiteSpace(s[i]))
        i++;
    while (s[i])
    {
        while (isWhiteSpace(s[i]))
            i++;
        val += s[i];
        i++;
        if (!s[i] or isWhiteSpace(s[i]))
        {
            if (key == "root")
                this->_root.second.push_back(val);
            else if (key == "index")
                this->_index.second.push_back(val);
            else
                error("Invalid Directive");
            val = "";
        }
    }
}

void Location::trimFile()
{
    size_t i = 0;
    size_t j = 0;
    std::string key;
    bool isInsideLocation = false;
    std::vector<std::string> newBuff;
    while (i < fileBuff.size())
    {
        j = 0;
        key.clear();
        while (fileBuff[i][j] && !isWhiteSpace(fileBuff[i][j]))
            key += fileBuff[i][j++];
        while (isWhiteSpace(fileBuff[i][j]))
            j++;
        if (key == "location")
        {
            collectPath(fileBuff[i]);
            isInsideLocation = true;
        }
        else
        {
            
            if (isInsideLocation)
            {
                fillDirective(fileBuff[i], key);
            }
        }
        if (fileBuff[i] == "}")
            isInsideLocation = false;
        i++;
    }
}

void Location::parseLocation()
{
    trimFile();
}