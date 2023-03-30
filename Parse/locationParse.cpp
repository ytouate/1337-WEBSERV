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
    : _start(i), _fileBuff(a), autoIndex(OFF) {}

void locationParse::fillDirective(const std::string &s, const std::string &key)
{
    size_t i = 0;
    std::string val;
    std::vector<std::string> values;

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
            values.push_back(val);
            val = "";
        }
    }
    if (key == "root")
        _root = std::make_pair(key, values);
    else if (key == "index")
        _index = std::make_pair(key, values);
    else if (key == "error_page")
    {
        if (values.size() < 2)
            error("Invalid Arguments");
        if (!isNumber(values.front()))
            error("Invalid Arguments");
        this->errorPages.insert(std::make_pair(atoi(values.front().c_str()),
                                               values.back()));
    }
    else if (key == "allowed_methods")
    {
        _allowed_methods = std::make_pair(key, values);
    }
    else if (key == "auto_index")
    {
        if (values.size() != 1)
            error("Invalid arguments");
        if (values.front() != "on" and values.front() != "off")
            error("Invalid Arguments");
        autoIndex = values.front() == "on" ? ON : OFF;
    }
    else if (key == "upload")
    {
        if (values.size() != 1)
            error("Invalid arguments");
        if (values.front() != "on" and values.front() != "off")
            error("Invalid Arguments");
        _upload = std::make_pair(key, values);
    }
}

locationParse::~locationParse()
{
    std::map<std::string, std::vector<std::string> >::iterator it = this->data.begin();
    while (it != data.end())
    {
        it->second.clear();
        ++it;
    }
    this->data.clear();
}

bool locationParse::isNumber(const std::string &s)
{
    for (size_t i = 0; i < s.size(); i++)
    {
        if (!isdigit(s[i]))
            return false;
    }
    return true;
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
    data.insert(this->_allowed_methods);
}