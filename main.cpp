#include "inc.hpp"
#include "Parse/Config.hpp"
#include "Response/Response.hpp"

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
//     requestParse request("POST /api/users HTTP/1.1\n\
// Host: example.com\n\
// User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36\n\
// Accept-Language: en-US\n\
// Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\n\
// Accept-Encoding: gzip, deflate\n\
// Connection: Keep-Alive\n\
// Content-Type: application/json\n\
// Content-Length: 56\r\n\r\n\
// \
// {\"username\": \"johndoe\", \"email\": \"johndoe@example.com\"}");

    requestParse request ("POST /api/upload HTTP/1.1\n\
Host: example.com\n\
Content-Type: multipart/form-data\n\
Content-Length: 229\r\n\r\n\
\
--------------------------7d42a317ba4f4\n\
Content-Disposition: form-data; name=\"file\"; filename=\"example.txt\"\n\
Content-Type: text/plain\n\
\
This is an example file.\n\
It can contain any type of text.\n\
\
--------------------------7d42a317ba4f4--\n");

    Config config(av[1]);
    if (config.servers[0].data["upload"].size() > 0 && config.servers[0].data["upload"].front() == "on")
        std::cout << "here\n";
    // std::cout << request.data["method"] << std::endl;
    // std::cout << request.data["version"] << std::endl;
    // std::cout << request.data["host"] << std::endl;
    // std::cout << request.data["path"] << std::endl;
    // std::cout << request.data["content-type"] << std::endl;
    // std::cout << request.data["content-length"] << std::endl;
    // std::cout << request.data["body"] << std::endl;
}
