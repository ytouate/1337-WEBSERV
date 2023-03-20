#if !defined(SERVER_HPP)
#define SERVER_HPP

#include "../inc.hpp"
#include "locationParse.hpp"


class locationParse;
class serverParse
{
private:
    typedef std::pair<std::string, std::vector<std::string> >  directive;

    int                         _start;
    std::string                 _lastKey;
    bool                        _serverIsOpened;
    bool                        _locationIsOpened;
    bool                        _isInsideServer;
    directive                   _root;
    directive                   _index;
    directive                   _allowedMethods;
    directive                   _port;
    directive                   _serverName;
    directive                   _serverIndex;
    directive                   _serverRoot;
    std::vector<std::string>    _fileBuff;
    std::vector<std::string>    _locationBuff;

    void            error(const std::string &a) const;
    void            fillDirective(const std::string &, const std::vector<std::string> &);
    void            handleErrors(const std::string &);
    void            setFileBuff(const std::vector<std::string> &);
    void            getValues(std::vector<std::string > &values, const std::string &fileBuff, int &j);
    bool            isWhiteSpace(char c);
    int             parseBlock();
    std::string     trim(const std::string &s);
    std::string     getKey(const std::string &fileBuff, int &j);

    friend class Config;
public:
    std::vector<locationParse>                                  locations;
    std::map<std::string, std::vector<std::string> >            data;

    serverParse(const std::vector<std::string> &, int);
    ~serverParse();
};

#endif // SERVER_HPP
