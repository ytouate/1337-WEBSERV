#include <iostream>

#include <vector>
#include <map>
#include <fstream>


#include <list>
#include <vector>
#include <map>
#include "lexer.hpp"

int error(const std::string &_err)
{
    std::cerr << _err << std::endl;
    return -1;
}

int main(int ac, char **av)
{
    if (ac != 2)
        return error("Not Enough Arguments");
    std::fstream configFile;
    token *token = NULL;
    configFile.open(av[1], std::fstream::in);
    std::string line;
    std::string Buff;
    while (std::getline(configFile, line))
    {
        Buff += line;
    }
    lexer lexer(Buff);
    while ((token = lexer.lexer_get_next_token()) != NULL)
    {
        std::cout << "TOKEN(" << token->type << ", " << token->content << ");"<<  std::endl;
        
    }
}