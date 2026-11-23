#include "tokenizer.hpp"
#include <cctype>
#include <string.h>

errorId_t tokenize(const std::string& input, std::vector<Token>& tokens) {
    errorId_t status = errorId_t::ERR_STATUS_OK;
    size_t i = 0;
    int line = 1;
    size_t lineStart = 0;

    if (!tokens.empty()) {
        status = errorId_t::ERR_TOKINIZER_NOT_EMPTY;
    }

    while ((i < input.size()) && (status == errorId_t::ERR_STATUS_OK)) {
        Token token;
        token.line = line;                       // position of the token START
        token.col = (int)(i - lineStart) + 1;
        unsigned char c = input[i];

        if (isspace(c)) {
            if (c == '\n') {
                line++;
                lineStart = i + 1;
            }
            i++;  // skip whitespaces
            continue;
        } else if(c == '.'){
            size_t start = i;
            i++;
            while(i < input.size() && isdigit((unsigned char) input[i])) {
                i++;
            }
            token.text = input.substr(start, i - start);
            token.type = TokenType::Float;
            if (i < input.size() &&
                (isalpha((unsigned char)input[i]) || input[i] == '_' ||
                 input[i] == '.')) {
                token.type = TokenType::Error;
                token.text = "malformed number: " +
                             input.substr(start, i - start + 1);
                status = errorId_t::ERR_TOKINIZER_INVALID_ENTRY;
            }
        } else if (isdigit(c)) {
            token.type = TokenType::Integer;
            size_t start = i;
            while (i < input.size() && isdigit((unsigned char)input[i])) {
                i++;
            }

            if (i < input.size() && input[i] == '.') {
                token.type = TokenType::Float;
                i++;
                while (i < input.size() && isdigit((unsigned char)input[i])) {
                    i++;
                }
            }

            token.text = input.substr(start, i - start);

            // malformed number: glued to a letter, '_' or a second '.'
            if (i < input.size() &&
                (isalpha((unsigned char)input[i]) || input[i] == '_' ||
                 input[i] == '.')) {
                token.type = TokenType::Error;
                token.text = "malformed number: " +
                             input.substr(start, i - start + 1);
                status = errorId_t::ERR_TOKINIZER_INVALID_ENTRY;
            }
        } else if (c != '\0' && strchr("+-*/%=<>!|", c)) {
            char next = (i + 1 < input.size()) ? input[i + 1] : '\0';

            switch (input[i]) {
            case '+':
                token.type = TokenType::Plus;
                token.text = "+";
                i++;
                break;
            case '-':
                token.type = TokenType::Minus;
                token.text = "-";
                i++;
                break;
            case '/':
                token.type = TokenType::Slash;
                token.text = "/";
                i++;
                break;
            case '*':
                token.type = TokenType::Star;
                token.text = "*";
                i++;
                break;
            case '%':
                token.type = TokenType::Percent;
                token.text = "%";
                i++;
                break;
            case '=':
                if (next == '=') {
                    token.text = "==";
                    i += 2;
                } else {
                    token.text = "=";
                    i++;
                }
                token.type = TokenType::Eq;
                break;
            case '<':
                if (next == '=') {
                    token.text = "<=";
                    token.type = TokenType::LtEq;
                    i += 2;
                } else if (next == '>') {
                    token.text = "<>";
                    token.type = TokenType::NotEq;
                    i += 2;
                } else {
                    token.text = '<';
                    token.type = TokenType::Lt;
                    i++;
                }
                break;
            case '>':
                if (next == '=') {
                    token.text = ">=";
                    token.type = TokenType::GtEq;
                    i += 2;
                } else {
                    token.text = '>';
                    token.type = TokenType::Gt;
                    i++;
                }
                break;
            case '!':
                if (next == '=') {
                    token.text = "!=";
                    token.type = TokenType::NotEq;
                    i += 2;
                } else {
                    token.text = "unexpected '!'";
                    token.type = TokenType::Error;
                    status = errorId_t::ERR_TOKINIZER_INVALID_ENTRY;
                }
                break;
            case '|':
                if (next == '|') {
                    token.text = "||";
                    token.type = TokenType::Concat;
                    i += 2;
                } else {
                    token.text = "unexpected '|'";
                    token.type = TokenType::Error;
                    status = errorId_t::ERR_TOKINIZER_INVALID_ENTRY;
                }
                break;
            default:
                token.text = "undefined !";
                token.type = TokenType::Error;
                status = errorId_t::ERR_TOKINIZER_INVALID_ENTRY;
                break;
            }
        } else if (isalpha(c) || c == '_') {
            size_t start = i;
            while (i < input.size() &&
                   (isalnum((unsigned char)input[i]) || input[i] == '_')) {
                i++;
            }

            std::string word = input.substr(start, i - start);
            std::string upper = word;
            for (char& ch : upper) {
                ch = toupper((unsigned char)ch);
            }

            auto it = KEYWORDS.find(upper);
            if (it != KEYWORDS.end()) {
                token.type = it->second;
                token.text = word;  // keep same case
            } else {
                token.type = TokenType::Identifier;
                token.text = word;
            }
        } else if (c == '(' || c == ')' || c == ',' || c == ';' || c == '.') {
            switch (c) {
            case '(':
                token.type = TokenType::LParen;
                break;
            case ')':
                token.type = TokenType::RParen;
                break;
            case ',':
                token.type = TokenType::Comma;
                break;
            case ';':
                token.type = TokenType::Semicolon;
                break;
            case '.':
                token.type = TokenType::Dot;
                break;
            }
            token.text = std::string(1, (char)c);
            i++;
        } else {
            token.type = TokenType::Error;
            token.text = std::string("unexpected character: ") + (char)c;
            status = errorId_t::ERR_TOKINIZER_INVALID_ENTRY;
        }

        tokens.push_back(token);
    }

    if (status == errorId_t::ERR_STATUS_OK) {
        Token eof;
        eof.type = TokenType::EndOfFile;
        eof.text = "";
        eof.line = line;                          // where the input ended
        eof.col = (int)(i - lineStart) + 1;
        tokens.push_back(eof);
    }

    return status;
}
