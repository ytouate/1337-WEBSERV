#if !defined(TOKEN_HPP)
#define TOKEN_HPP

#include <iostream>
# define TOKEN_ID 1
# define TOKEN_EQUALS 2
# define TOKEN_STRING 3
# define TOKEN_SEMI 4
# define TOKEN_LPAREN 5
# define TOKEN_RPAREN 6
# define TOKEN_LCURLYPAREN 7
# define TOKEN_RCURLYPARENT 8
struct token
{
    std::string content;
    int type;
    token(int type, const std::string &content);
};

#endif // TOKEN_HPP
