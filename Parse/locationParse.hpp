#if !defined(locationParse_HPP)
#define locationParse_HPP

#include "../inc.hpp"
#include "Config.hpp"

#define  ON 1
#define  OFF 0

class locationParse
{
private:
    typedef std::pair<std::string, std::vector<std::string> > directive;

    directive                           _root;
    directive                           _index;
    directive                           _allowed_methods;
    directive                           _upload;
    directive                           _upload_path;
    int                                 _start;
    std::vector<std::string>            _fileBuff;

    void        setDirective(const std::string &key, std::vector<std::string> &values);
    bool        isWhiteSpace(char c);
    void        error(const std::string &a) const;
    void        collectPath(const std::string &s);
    void        parseBlock();
    void        fillDirective(const std::string &s, const std::string &key);
    bool        isNumber(const std::string &s);
    friend class serverParse;
public:
    std::string                                         path;
    std::map<std::string, std::vector<std::string> >    data;
    std::map<int, std::string>                          errorPages;
    bool                                                autoIndex;
    locationParse(const std::vector<std::string> &, int);
    ~locationParse();
};

#endif // locationParse_HPP
