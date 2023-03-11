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
Location::Location(const std::vector<std::string> &a, int i)
{
    start = i;
    fileBuff = a;
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
    std::cout << s << std::endl;
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
            {
                this->_root.first = key;
                this->_root.second.push_back(val);
            }
            else if (key == "index")
            {
                this->_index.first = key;
                this->_index.second.push_back(val);
            }
            else
                error("Invalid Directive");
            val = "";
        }
    }
}


void Location::parseLocation()
{
    size_t i = start;
    size_t j = 0;
    std::string key;
    bool isInsideLocation = false;
    std::vector<std::string> newBuff;
    while (i < fileBuff.size() && fileBuff[i] != "}")
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