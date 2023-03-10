#include "Lexer.hpp"

Lexer::Lexer(const std::string &_content)
{
    this->i = 0;
    this->content = _content;
    this->c = this->content[this->i];
}
void Lexer::lexer_advance()
{
    if (this->c && this->i < this->content.size())
    {
        this->i++;
        this->c = this->content[this->i];
    }
}
void Lexer::lexer_skip_whitespace()
{
    while (this->c == ' ' or this->c == '\n' or this->c == '\t')
        this->lexer_advance();
}
Token *Lexer::lexer_get_next_token()
{
    while (this->c and this->i < this->content.size())
    {
        if (this->c == ' ' or this->c == '\n' or this->c == '\t')
            this->lexer_skip_whitespace();
        if (isalnum(this->c) or this->c == '_' or this->c == '/' or this->c == '.')
            return lexer_collect_id();
        else if (this->c == '"')
            return lexer_collect_string();
        else if (this->c == '=')
            return lexer_advance_with_token((new Token(TOKEN_EQUALS,
                                                       lexer_get_current_char_as_string())));
        else if (this->c == '{')
            return lexer_advance_with_token((new Token(TOKEN_LCURLYPAREN,
                                                       lexer_get_current_char_as_string())));
        else if (this->c == '}')
            return lexer_advance_with_token((new Token(TOKEN_RCURLYPARENT,
                                                       lexer_get_current_char_as_string())));
        else if (this->c == ';')
            return lexer_advance_with_token((new Token(TOKEN_SEMI,
                                                       lexer_get_current_char_as_string())));
        else if (this->c == '(')
            return lexer_advance_with_token((new Token(TOKEN_LPAREN,
                                                       lexer_get_current_char_as_string())));
        else if (this->c == ')')
            return lexer_advance_with_token((new Token(TOKEN_RPAREN,
                                                       lexer_get_current_char_as_string())));
        else
            return NULL;
    }

    return NULL;
}

Token *Lexer::lexer_collect_id()
{
    std::string value;
    while (isalnum(this->c) or this->c == '/' or this->c == '_' or this->c == '.')
    {

        std::string s = lexer_get_current_char_as_string();
        value += s;
        lexer_advance();
    }
    return new Token(TOKEN_STRING, value);
}
Token *Lexer::lexer_collect_string()
{
    lexer_advance();
    std::string value;
    while (this->c != '"')
    {
        std::string s = lexer_get_current_char_as_string();
        value += s;
        lexer_advance();
    }
    lexer_advance();
    return new Token(TOKEN_STRING, value);
}
std::string Lexer::lexer_get_current_char_as_string()
{
    std::string str;
    str += this->c;
    str += '\0';
    return str;
}
Token *Lexer::lexer_advance_with_token(Token *_token)
{
    lexer_advance();
    return _token;
}