#include "serverParse.hpp"

serverParse::serverParse(const std::vector<std::string> &__fileBuff, int __start)
{
    _lastKey = "";
    _serverIsOpened = _locationIsOpened = false;
    _isInsideServer = false;
    _fileBuff = __fileBuff;
    _start = __start;
}

bool isCurlyBracket(const std::string &s)
{
    return (s == "{" or s == "}");
}

void serverParse::handleErrors(const std::string &_fileBuff)
{
    if (isCurlyBracket(_fileBuff) or _fileBuff == "server")
    {
        if (_fileBuff == "{")
        {
            if (_lastKey == "server")
                _serverIsOpened = true;
            else if (_lastKey == "location")
                _locationIsOpened = true;
            else
                error("invalid config file");
        }
        else if (_fileBuff == "}")
        {
            if (_locationIsOpened)
                _locationIsOpened = false;
            else if (_serverIsOpened)
                _serverIsOpened = false;
            else
                error("invalid config file");
        }
    }
}

std::string serverParse::getKey(const std::string &_fileBuff, int &j)
{
    std::string key;
    while (_fileBuff[j] && !isWhiteSpace(_fileBuff[j]))
        key += _fileBuff[j++];
    return key;
}

void serverParse::getValues(std::vector<std::string> &values,
                       const std::string &_fileBuff, int &j)
{
    std::string val;
    while (_fileBuff[j])
    {
        while (_fileBuff[j] && isWhiteSpace(_fileBuff[j]))
            j++;
        val += _fileBuff[j];
        j++;
        if (!_fileBuff[j] or isWhiteSpace(_fileBuff[j]))
        {
            values.push_back(val);
            val = "";
        }
    }
}

int serverParse::parseBlock()
{
    size_t i = _start;

    int j = 0;
    std::string key;
    std::string val;
    int locationsCount = 0;
    std::vector<std::string> values;
    _isInsideServer = false;
    while (i < _fileBuff.size())
    {
        _isInsideServer = !_locationIsOpened;
        j = 0;
        values.clear();
        _lastKey = key;
        val = key = "";
        handleErrors(_fileBuff[i]);
        key = getKey(_fileBuff[i], j);
        if ((key == "server" && _fileBuff[i + 1] != "{") ||
            (key == "location" && _fileBuff[i + 1] != "{"))
            error("block not opened");
        if (key == "location")
        {
            if (_locationIsOpened)
                error("unclosed block");
            locations.push_back(locationParse(this->_fileBuff, i));
            locations[locationsCount].parseBlock();
            locationsCount++;
        }
        getValues(values, _fileBuff[i], j);
        fillDirective(key, values);
        i++;
        if (key == "}" and _lastKey == "}")
            break;
    }
    for (size_t i = 0; i < locations.size(); i++)
    {
        if (locations[i].data["root"].size() == 0)
        {
            std::cout<< "hna\n";
            locations[i].data["root"] = this->_serverRoot.second;
        }
        else if (locations[i].data["index"].size() == 0)
            locations[i].data["index"] = this->_serverIndex.second;
    }
    if (_serverIsOpened or _locationIsOpened)
        error("block not closed");
    data.insert(_index);
    data.insert(_root);
    data.insert(_port);
    data.insert(_serverName);
    return i;
}

bool serverParse::isWhiteSpace(char c)
{
    return (c == ' ' or c == '\t');
}

void serverParse::error(const std::string &s) const
{
    std::cerr << s << std::endl;
    exit(EXIT_FAILURE);
}
void serverParse::fillDirective(const std::string &key,
                           const std::vector<std::string> &values)
{
    if (key == "listen")
        _port = std::make_pair(key, values);
    else if (key == "root")
    {
        if (_isInsideServer)
            this->_serverRoot = std::make_pair(key, values);
        _root = std::make_pair(key, values);
    }
    else if (key == "index")
    {
        if (_isInsideServer)
            this->_serverIndex = std::make_pair(key, values);
        _index = std::make_pair(key, values);
    }
    else if (key == "server_name")
    {
        _serverName = std::make_pair(key, values);
    }
    else
    {
        if (key != "location" && key != "server" && !isCurlyBracket(key))
            error("invalid directive");
    }
}

std::string serverParse::trim(const std::string &s)
{
    std::string trimmed;
    int _start = 0;
    int end = s.size() - 1;
    if (end < 0)
        return "";
    while (s[_start] && isWhiteSpace(s[_start]))
        _start++;
    while (end > 0 && isWhiteSpace(s[end]))
        end--;
    while (_start <= end)
        trimmed += s[_start++];
    return trimmed;
}

serverParse::~serverParse() {}
