#include "inc.hpp"
#include <map>
#include <iostream>
#include <vector>
#include "Parse/Config.hpp"
#include "Response/Response.hpp"
#include <deque>
#include <fcntl.h>

typedef std::map<std::string, std::vector<std::string> > directive;

void printDrective(directive &a)
{
    std::cout << "root:\t";
    for (size_t i = 0; i < a["root"].size(); ++i)
        std::cout << " " << a["root"][i];
    std::cout << "\n";
    std::cout << "index:\t";
    for (size_t i = 0; i < a["index"].size(); ++i)
        std::cout << " " << a["index"][i];
    std::cout << "\n";
    std::cout << "listen:\t";
    for (size_t i = 0; i < a["listen"].size(); ++i)
        std::cout << " " << a["listen"][i];
    std::cout << "\n";
    std::cout << "server_name:\t";
    for (size_t i = 0; i < a["server_name"].size(); ++i)
        std::cout << " " << a["server_name"][i];
    std::cout << "\n";
}

int main(int ac, char **av)
{
    if (ac != 2)
        return 1;
    requestParse request(
        "POST /php.php  HTTP/1.1\n\
        User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36\n\
        Accept-Language: en-US\n\
        Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\n\
        Accept-Encoding: gzip, deflate\n\
        Host: localhost:9001\n\
        Connection: Keep-Alive\n\
        "
    );
    Config config(av[1]);
    Response response(config, request);
    // std::cout << response._response << std::endl;
}
