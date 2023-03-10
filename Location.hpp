#if !defined(LOCATION_HPP)
#define LOCATION_HPP

#include <iostream>
#include <set>

struct Location
{
    std::string path;
    std::map<std::string, std::set<std::string> > directives;
};


#endif // LOCATION_HPP
