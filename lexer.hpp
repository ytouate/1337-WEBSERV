#if !defined(LEXER_HPP)
#define LEXER_HPP

#include <iostream>
#include "token.hpp"

struct lexer
{
    char c;
    size_t i;
    std::string content;

    lexer(const std::string &content);
    void lexer_advance();
    void lexer_skip_whitespace();
    token *lexer_get_next_token();
    token *lexer_collect_string(); 
    std::string lexer_get_current_char_as_string();
    token *lexer_advance_with_token(token *_token);
    token *lexer_collect_id();
};

#endif // LEXER_HPP
