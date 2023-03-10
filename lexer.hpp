#if !defined(LEXER_HPP)
#define LEXER_HPP

#include <iostream>
#include "Token.hpp"

struct Lexer
{
    char c;
    size_t i;
    std::string content;

    Lexer(const std::string &content);
    void lexer_advance();
    void lexer_skip_whitespace();
    Token *lexer_get_next_token();
    Token *lexer_collect_string();
    std::string lexer_get_current_char_as_string();
    Token *lexer_advance_with_token(Token *_token);
    Token *lexer_collect_id();
};

#endif // LEXER_HPP
