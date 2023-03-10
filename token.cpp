#include "token.hpp"

token::token(int _type, const std::string &_content)
{
    this->type = _type;
    this->content = _content;
}