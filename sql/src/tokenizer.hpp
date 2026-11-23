#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include "common/utils.hpp"

enum class TokenType {
    // literals
    Integer, Float, String, Blob, Null,
    // names
    Identifier,
    // keywords
    Select, From, Where, Insert, Into, Values, Create, Table, Primary, Key,
    Not, And, Or, KwInteger, KwText, KwReal, KwBlob,
    // operators
    Plus, Minus, Star, Slash, Percent, Eq, NotEq, Lt, LtEq, Gt, GtEq, Concat,
    // punctuation
    LParen, RParen, Comma, Semicolon, Dot,
    // special
    Parameter, EndOfFile, Error
};

static const std::unordered_map<std::string, TokenType> KEYWORDS = {
    {"SELECT", TokenType::Select},   {"FROM", TokenType::From},
    {"WHERE", TokenType::Where},     {"INSERT", TokenType::Insert},
    {"INTO", TokenType::Into},       {"VALUES", TokenType::Values},
    {"CREATE", TokenType::Create},   {"TABLE", TokenType::Table},
    {"PRIMARY", TokenType::Primary}, {"KEY", TokenType::Key},
    {"NOT", TokenType::Not},         {"NULL", TokenType::Null},
    {"AND", TokenType::And},         {"OR", TokenType::Or},
    {"INTEGER", TokenType::KwInteger}, {"TEXT", TokenType::KwText},
    {"REAL", TokenType::KwReal},     {"BLOB", TokenType::KwBlob},
};

struct Token {
    TokenType type;
    std::string text;   // raw lexeme or decoded value
    int line, col;      // for error messages
};

errorId_t tokenize(const std::string& input, std::vector<Token>& tokens);

#endif /* TOKENIZER_HPP */
