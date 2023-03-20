#if !defined(LOCATION_HPP)
#define LOCATION_HPP

#include "inc.hpp"
#include "Config.hpp"

class Location
{
private:
    typedef std::pair<std::string, std::vector<std::string> > directive;

    directive                           _root;
    directive                           _index;
    directive                           _allowed_methods;
    directive                           _port;
    int                                 _start;
    std::vector<std::string>            _fileBuff;

    bool        isWhiteSpace(char c);
    void        error(const std::string &a) const;
    void        collectPath(const std::string &s);
    void        parseBlock();
    void        fillDirective(const std::string &s, const std::string &key);

    friend class Server;
public:
    std::string                                         path;
    std::map<std::string, std::vector<std::string> >    data;
    Location(const std::vector<std::string> &, int);
    ~Location();
};

#endif // LOCATION_HPP
