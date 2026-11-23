#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <string>
#include <vector>
#include <common/utils.hpp>

enum class TokenType {
    // literals
    Integer, Float, String, Blob, Null,
    // names
    Identifier, Keyword,
    // operators
    Plus, Minus, Star, Slash, Percent, Eq, NotEq, Lt, LtEq, Gt, GtEq, Concat,
    // punctuation
    LParen, RParen, Comma, Semicolon, Dot,
    // special
    Parameter, EndOfFile, Error
};

struct Token {
    TokenType type;
    std::string text;   // raw lexeme or decoded value
    int line, col;      // for error messages
};

std::vector<Token> tokenize(const std::string& input);

#endif /* TOKENIZER_HPP */
