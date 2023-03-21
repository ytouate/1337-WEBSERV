#include "locationParse.hpp"

void locationParse::collectPath(const std::string &s)
{
    size_t i = 0;
    while (s[i] && !isWhiteSpace(s[i]))
        i++;
    while (s[i] && isWhiteSpace(s[i]))
        i++;
    while (s[i])
        path += s[i++];
}

bool locationParse::isWhiteSpace(char c)
{
    return (c == ' ' or c == '\t');
}

void locationParse::error(const std::string &s) const
{
    std::cerr << s << std::endl;
    exit(EXIT_FAILURE);
}
locationParse::locationParse(const std::vector<std::string> &a, int i)
    : _start(i), _fileBuff(a) {}
void locationParse::fillDirective(const std::string &s, const std::string &key)
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

locationParse::~locationParse()
{
    this->data.clear();
}
void locationParse::parseBlock()
{
    size_t i = _start;
    size_t j = 0;
    std::string key;
    bool isInsideLocation = false;
    std::vector<std::string> newBuff;
    while (i < _fileBuff.size() && _fileBuff[i] != "}")
    {
        j = 0;
        key.clear();
        while (_fileBuff[i][j] && !isWhiteSpace(_fileBuff[i][j]))
            key += _fileBuff[i][j++];
        while (_fileBuff[i][j] && isWhiteSpace(_fileBuff[i][j]))
            j++;
        if (key == "location")
        {
            collectPath(_fileBuff[i]);
            isInsideLocation = true;
        }
        else
        {
            if (isInsideLocation)
                fillDirective(_fileBuff[i], key);
        }
        if (_fileBuff[i] == "}")
            isInsideLocation = false;
        i++;
    }
    data.insert(this->_root);
    data.insert(this->_index);
}